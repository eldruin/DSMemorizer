/// \file textboxhandler.cpp Text boxes handler functions.
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
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <vector>
using std::string;
using std::vector;

#include "utf8.h"

#include "types.h"
#include "textbox.h"
#include "textboxhandler.h"
#include "files.h"

void TextBoxHandler::Init()
{
  if (FT_Init_FreeType (&library_))
    iprintf ("Error ocurred in FreeType load. ");
  faces_[Types::MONA_FONT] = NULL; // New NULL initializing line is needed
                                   // for each supported font
}

TextBox* TextBoxHandler::NewTextBox (int bgid, Types::Font font, int size,
                                     int x, int y, int width, int height)
{
  if (faces_[font] == NULL)
  {
    // Font not loaded, it's loaded now
    FT_Face face;
    int error = FT_New_Face (library_, Files::filepath(font).c_str(), 0, &face);
    if (error == FT_Err_Unknown_File_Format)
    {
      iprintf ("the font file could be opened and read, but it appears that "
               "its font format is unsupported.");
    }
    else if (error)
    {
      iprintf ("the font file could not be opened or read, or it's broken.");
    }
    else
      // OK
      faces_[font] = face;
  }
  TextBox* tb = new TextBox ();
  tb->Init(bgid, faces_[font], size, x, y, width, height);
  text_boxes_.push_back(tb);
  return tb;
}

void TextBoxHandler::DestroyTextBox (TextBox* tb)
{
  size_t i = 0;
  for (; text_boxes_[i] != tb && i < text_boxes_.size(); ++i);
  if (text_boxes_[i] == tb)
  {
    delete tb;
    text_boxes_.erase(text_boxes_.begin()+i);
  }
}

void TextBoxHandler::PrintAll ()
{
  for (size_t i = 0; i < text_boxes_.size(); ++i)
  {
    text_boxes_[i]->Print();
    if (i < text_boxes_.size()-1)
      if (!text_boxes_[i]->floats() &&
          text_boxes_[i]->bgid() == text_boxes_[i+1]->bgid())
        text_boxes_[i+1]->Adjust(text_boxes_[i]->y());
  }
}

TextBoxHandler::~TextBoxHandler()
{
  for (size_t i = 0; i < faces_.size(); ++i)
    FT_Done_Face(faces_[i]);
  FT_Done_FreeType(library_);
}


