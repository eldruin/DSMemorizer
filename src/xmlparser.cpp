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
#include <cstring>
#include <cstdlib> // for atoi


#include "utf8.h"
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
      fseek(file_, 0, SEEK_SET);
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
        }
        // else bad file error
      }
      // else bad file error
    }
    // else bad file error
  }
  // else bad file error
}

Card XMLParser::card (int index)
{
  Card read_card;
  if (index && index <= package_records_)
  {
    char buff [BUFFER_SIZE];
    // Reads from the beginning. Index file could be generated and used here.
    fseek(file_, 0, SEEK_SET);
    // Reads "index" number of lines from the file plus 2 more of the heading.
    // Each card record MUST be in one line.
    // Lines must not be longer than 511 bytes
    for (int i = 0; i < index+2 && !feof(file_); ++i)
      fgets(buff, BUFFER_SIZE, file_);

    int position = 0;
    // Parse the string and create the card
    read_card.Init(index,
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

string XMLParser::attribute_value(const char* name, const char* buffer, int& position)
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

XMLParser::~XMLParser ()
{
  fclose(file_);
}


