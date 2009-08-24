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

inline u8 convert_color(u8 incolor)
{
	u8 outcolor;
	if (incolor > 250)
		outcolor = Types::Color::BLACK;
	else if (incolor > 128)
		outcolor = Types::Color::GREY;
	else
		outcolor = Types::Color::WHITE;
	return outcolor;
}

void TextBoxHandler::Init(int bgid)
{
  bgid_ = bgid;
  if (FT_Init_FreeType (&library_))
    iprintf ("Error ocurred in FreeType load. ");
  faces_[Types::MONA_FONT] = NULL; // New NULL initializing line is needed
                                   // for each supported font
}

TextBox* TextBoxHandler::New (Types::Font font, int size, int x, int y, int width,
                  int height)
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
  tb->Init(bgid_, faces_[font], size, x, y, width, height);
  text_boxes_.push_back(tb);
  return tb;
}

void TextBoxHandler::Destroy (TextBox* tb)
{
  size_t i = 0;
  for (; text_boxes_[i] != tb && i < text_boxes_.size(); ++i);
  if (text_boxes_[i] == tb)
  {
    delete tb;
    text_boxes_.erase(text_boxes_.begin()+i);
  }
}

void TextBoxHandler::Print ()
{
  iprintf("line height: %i\n", faces_[Types::MONA_FONT]->height);
  for (size_t i = 0; i < text_boxes_.size(); ++i)
  {
    text_boxes_[i]->Print();
    if (i < text_boxes_.size()-1)
      text_boxes_[i+1]->Adjust(text_boxes_[i]->y());
  }
}

TextBoxHandler::~TextBoxHandler()
{
  for (size_t i = 0; i < faces_.size(); ++i)
    FT_Done_Face(faces_[i]);
  FT_Done_FreeType(library_);
}

#if 0
void Text::LoadFace (Types::Font f, int size)
{
	size_ = size;
  int error;
  error = FT_New_Face (library_, Files::filepath(f).c_str(), 0, &face_);
  if (error == FT_Err_Unknown_File_Format)
  {
    iprintf ("the font file could be opened and read, but it appears that its font format is unsupported.");
  }
  else if (error)
  {
    iprintf ("the font file could not be opened or read, or it's broken.");
  }

  FT_Set_Char_Size (face_, size * 64, 0, 110, 110);
}

void Text::Print(const string& str, int& x, int& y)
{
  FT_Vector pen;
  pen.x = x;
  pen.y = y;

  string::const_iterator it = str.begin();
  while (it != str.end())
  {
    int glyph_index;
    u32 utf8character = utf8::unchecked::next(it);
    if (utf8character == 32) // space
    {
      pen.x += size_*2/3;
    }
    else
    {
      glyph_index = FT_Get_Char_Index (face_, utf8character);
      // Renders in 256 level gray
      FT_Load_Glyph (face_,	glyph_index, FT_LOAD_RENDER);

      // Added lots of local variables trying to speed up the rendering.
			int width = face_->glyph->bitmap.width;
  		int rows = face_->glyph->bitmap.rows;
  		int ycoord = pen.y - rows;
  		int penx = pen.x;
  		u8* buffer = face_->glyph->bitmap.buffer;

      // Copy the image from the rendered glyph bitmap to the video buffer
      for (int glyph_y = 0; glyph_y < rows; ++glyph_y)
      {
        for (int glyph_x = 0; glyph_x < width-1; glyph_x += 2)
        {
          int index = glyph_y * width + glyph_x;
          // Gets the color of two pixels, if the gray level is above 248,
          // prints black, else prints white. Also aligns the two bytes.
          u16 color = (((buffer[index]>>3) & 0x1f)>>4) | ((((buffer[index+1]>>3) & 0x1f)>>4)<<8);
          int video_index = (ycoord + glyph_y) * 128 + (penx + glyph_x) / 2;
          video_buffer_[video_index] = color;
        }
      }

      // increment pen position
      pen.x += face_->glyph->advance.x >> 6;
    }
  }

  x = pen.x;
}
#endif

