/// \file xmlparser.cpp XML files parser.
/* Copyright 2009 Diego Barrios Romero
 *
 * This file is part of DSMemorizer.
 *
 * DSMemorizer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DSMemorizer distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DSMemorizer.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <nds.h>
#include <stdio.h>
#include <malloc.h>
#include <fat.h>
#include <unistd.h>

#include "efs_lib.h"

#include <string>
#include <vector>
#include <cstring>
#include <cstdlib> // for atoi

#include "utf8.h"

#include "definitions.h"
#include "card.h"
#include "xmlparser.h"

using std::string;

XMLParser::XMLParser()
{
  file_ = NULL;
}

void XMLParser::Init(const string& file_path)
{
  if (file_)
  {
    if (file_path == file_path_)
      // If the file is already opened just reset the cursor
      fseek(file_, 0L, SEEK_SET);
    else
    {
      fclose(file_);
      file_ = fopen(file_path.c_str(),"rb");
    }
  }
  else
    file_ = fopen(file_path.c_str(),"rb");

  file_path_= file_path;
  char buff [BUFFER_SIZE];
  if (file_ && !feof(file_))
  {
    fgets(buff, BUFFER_SIZE, file_);
    string buffer(buff);
    if (buffer.compare("<?xml version=\"1.0\"?>"))
    {
      xml_version_ = "1.0";
      if (!feof(file_)){
        fgets(buff, BUFFER_SIZE, file_);
        int position = 0;
        if (find("<package", buff, position))
        {
          package_name_ = attribute_value ("name", buff, position);
          package_records_ = atoi(attribute_value("records", buff, position).c_str());
          package_format_ = attribute_value ("format", buff, position);
          package_license_ = attribute_value ("license", buff, position);

          file_cursor_ = new fpos_t [package_records_];
          int auxpos = 0;
          if (find("kanji", package_format_.c_str(), auxpos))
            file_format_ = XML_KANJI;
          if (find("vocabulary", package_format_.c_str(), auxpos))
            file_format_ = XML_VOCABULARY;
          if (file_format_ == XML_KANJI)
          {
            grade_ = new std::vector<int> [MAX_GRADE];
            strokes_ = new std::vector<int> [MAX_STROKES];
            q_grade_min_ = q_grade_max_ = q_strokes_min_ = q_strokes_max_ = 0;
          }
          GenerateIndexes();
        }
        // else bad file error
      }
      // else bad file error
    }
    // else bad file error
  }
  // else bad file error
}

Card XMLParser::card (unsigned index, unsigned grade_min, unsigned grade_max,
                      unsigned strokes_min, unsigned strokes_max)
{
  Card read_card;
  int file_index = GetIndex(index, grade_min, grade_max, strokes_min, strokes_max);
  if (file_index && file_index <= package_records_)
  {
    char buff [BUFFER_SIZE];
    fpos_t fposition = file_cursor_[file_index-1];
    fsetpos(file_, &fposition);
    // Each card record MUST be in one line.
    // Lines must not be longer than 511 bytes
    fgets(buff, BUFFER_SIZE, file_);

    int position = 0;
    // Parse the string and create the card
    read_card.Init(file_index,
                   attribute_value(" symbol",buff, position),
                   attribute_value(" reading",buff, position),
                   attribute_value(" reading2",buff, position),
                   attribute_value(" translation",buff, position),
                   attribute_value(" example_symbol",buff, position),
                   attribute_value(" example_reading",buff, position),
                   attribute_value(" example_translation",buff, position)
                  );
  }

  return read_card;
}

int XMLParser::package_records () const
{
  return package_records_;
}

int XMLParser::QueryResultSize (unsigned grade_min, unsigned grade_max,
                                unsigned strokes_min, unsigned strokes_max)
{
  QueryResult(grade_min, grade_max, strokes_min, strokes_max);

  return query_result_.size();
}

string XMLParser::attribute_value(const char* name, const char* buffer,
                                  int& position)
{
  string result;
  if (find(name, buffer, position))
  {
  	position += strlen(name);
    int start = position + 2;
    // skip "=\"" and reads until '\"'
    for (position += 2; buffer[position] != '\"' && buffer[position] != '\0'
         && position < BUFFER_SIZE; ++position);
    if (buffer[position] == '\"')
    {
      // when reads '\"' create the string of the value
      result = string(buffer+start, position-start);
    }
  }

  ++position; // skip '\"'

  return result;
}

void XMLParser::GenerateIndexes()
{
  char buff [BUFFER_SIZE];
  fseek(file_, 0L, SEEK_SET);
  fgets(buff, BUFFER_SIZE, file_);
  fgets(buff, BUFFER_SIZE, file_);
  for (int i = 0; i < package_records_ && !feof(file_); ++i)
  {
    fpos_t position;
    int auxpos = 0;
    fgetpos(file_, &position);
    fgets(buff, BUFFER_SIZE, file_);
    file_cursor_[i] = position;
    if (file_format_ == XML_KANJI)
    {
      grade_[atoi(attribute_value(" grade", buff, auxpos).c_str())-1].
        push_back(i);
      strokes_[atoi(attribute_value(" strokes", buff, auxpos).c_str())-1].
        push_back(i);
    }
  }
}

unsigned XMLParser::GetIndex(unsigned index, unsigned grade_min,
                             unsigned grade_max, unsigned strokes_min,
                             unsigned strokes_max)
{
  unsigned final_index;
  if (file_format_ == XML_KANJI)
  {
    QueryResult(grade_min, grade_max, strokes_min, strokes_max);

    --index;
    if (index < query_result_.size())
      final_index = query_result_[index]+1;
    else
      final_index = 0;
  }
  else
    final_index = index;

  return final_index;
}

void XMLParser::QueryResult (unsigned grade_min, unsigned grade_max,
                             unsigned strokes_min, unsigned strokes_max)
{
  // if the any number of the range has changed, generates the
  // query result vector again.
  if (grade_min != q_grade_min_ || grade_max != q_grade_max_ ||
      strokes_min != q_strokes_min_ || strokes_max != q_strokes_max_)
  {
    q_grade_min_ = grade_min, q_grade_max_ = grade_max;
    q_strokes_min_ = strokes_min, q_strokes_max_ = strokes_max;
    query_result_.clear();
    --grade_min;
    --strokes_min;
    for (unsigned i = grade_min; i < grade_max; ++i)
      for (unsigned j = strokes_min; j < strokes_max; ++j)
        for (unsigned k = 0; k < grade_[i].size(); ++k)
        {
          unsigned l = 0;
          for (; l < strokes_[j].size() &&
               strokes_[j][l] != grade_[i][k]; ++l);

          if (l < strokes_[j].size())
            query_result_.push_back(grade_[i][k]);
        }
  }
}

XMLParser::~XMLParser ()
{
  fclose(file_);
}

