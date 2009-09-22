/// \file textbox.cpp Text box class.
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

#include <string>
#include "utf8.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "types.h"
#include "textbox.h"
#include "graphics.h"

using std::string;

void TextBox::Init (Types::Screen::selector screen,
                    int bgid, FT_Face face, int size, int x, int y,
                    int width, int height)
{
  screen_ = screen;
  bgid_ = bgid;
  face_ = face;
  size_ = size;
  x_ = x;
  y_ = y;
  width_ = (width==0) ? SCREEN_WIDTH - x - 20 : width;
  height_ = (height==0) ? ((size_*3)>>1) : height;
  mutable_height_ = (height == 0);
  floats_ = independent_ = false;
  visible_ = true;
}

std::string TextBox::text () const
{
  return text_;
}

int TextBox::size () const
{
  return size_;
}

int TextBox::x () const
{
  return x_;
}

int TextBox::y () const
{
  return y_;
}

int TextBox::width () const
{
  return width_;
}

int TextBox::height () const
{
  return height_;
}

int TextBox::bgid () const
{
  return bgid_;
}

bool TextBox::floats () const
{
  return floats_;
}

bool TextBox::independent () const
{
  return independent_;
}

bool TextBox::visible () const
{
  return visible_;
}

Types::Screen::selector TextBox::screen () const
{
  return screen_;
}

void TextBox::floats (bool f)
{
  floats_ = f;
}

void TextBox::independent (bool i)
{
  independent_ = i;
}

void TextBox::visible (bool v)
{
  visible_ = v;
}

void TextBox::SetProperties (int x, int y, int width, int height)
{
  Move(x,y);
  width_ = (width==0) ? SCREEN_WIDTH - x - 10 : width;
  height_ = (height==0) ? ((size_*3)>>1) : height;
}

void TextBox::text (const std::string& str, bool convert_to_romaji)
{
  if (convert_to_romaji)
    text_ = ConvertToRomaji(str);
  else
    text_ = str;
}

void TextBox::Move (int x, int y)
{
  x_ = x;
  y_ = y;
}

void TextBox::Adjust (int y)
{
  // The ((size_*3)>>1)-5 is because the points are a distance of 1/72 of
  // an inch in freetype library, but the DS screen resolution is 110 dpi,
  // so 110/72 is aproximately 1.5
  // And 5 extra pixels of line separation
  if (y > (y_ - 5))
  {
    y_ = y +5;
  }
}

void TextBox::Print ()
{
  // set the size because it could have been changed by another textbox sharing
  // the same face
  FT_Set_Char_Size (face_, size_ << 6, 0, 110, 110);

  int pen_x = x_;
  int pen_y = y_ + ((size_*3)>>1);
  string::const_iterator it = text_.begin();
  while (it != text_.end())
  {
    int glyph_index;
    u32 utf8character = utf8::unchecked::next(it);
    if (utf8character == 32) // space
    {
      pen_x += size_*2/3;
      if ((pen_x - x_) > width_)
      { // New line
        pen_y += face_->height>>6;
        pen_x = x_;
        if ((pen_y - y_) > height_)
        {
          // vertically out of the box
          if (mutable_height_)
            height_ = pen_y - y_;
          else
            return; // ERROR can't print a line more
        }
      }
    }
    else
    {
      glyph_index = FT_Get_Char_Index (face_, utf8character);
      // Renders in 256 level gray
      FT_Load_Glyph (face_,	glyph_index, FT_LOAD_RENDER);

      pen_x += face_->glyph->metrics.horiBearingX>>6;
      if (visible_)
      {
        // Copy the image from the rendered glyph bitmap to the video buffer
        Graphics::PrintBitmap (pen_x,
                               pen_y - (face_->glyph->metrics.horiBearingY>>6),
                               face_->glyph->bitmap.width,
                               face_->glyph->bitmap.rows,
                               face_->glyph->bitmap.buffer, bgid_, screen_);
      }
      // increment pen position
      pen_x += (face_->glyph->advance.x >> 6) -
               (face_->glyph->metrics.horiBearingX >> 6);
      if ((pen_x - x_) > width_)
      { // New line
        pen_y += face_->height >> 6;
        pen_x = x_;
        if ((pen_y - y_) > height_)
        {
          // vertically out of the box
          if (mutable_height_)
            height_ = pen_y - y_;
          else
            return; // ERROR can't print a line more
        }
      }
    }
  }
}

/// Kana to romaji conversion table
const char* kana_table =
							"a  a  i  i  u  u  e  e  o  o  ka ga ki gi ku gu "
					    "ke ge ko go sa za shiji su zu se ze so zo ta da "
							"chiji tsutsutzute de to do na ni nu ne no ha ba "
							"pa hi bi pi fu bu pu he be pe ho bo po ma mi mu "
							"me mo ya ya yu yu yo yo ra ri ru re ro wa wa wi "
							"we wo n  ";

/// Take out spaces of a string
inline string no_spaces(const string& str)
{
	string no_spaces_str;
	for (size_t i = 0; i < str.size(); ++i)
		if (str[i] != ' ')
			no_spaces_str.push_back(str[i]);

	return no_spaces_str;
}

string TextBox::ConvertToRomaji (string str)
{
	string romaji;
	string::iterator it = str.begin();
	uint32_t prev_utf8c = 0;
	uint32_t utf8c = 0;
	uint32_t kana_base = 0;
	uint32_t prev_kana_base = 0;
  while (it != str.end())
  {
  	if (prev_utf8c)
  	{
  		if (prev_kana_base)
	  		romaji += no_spaces(string(kana_table+((prev_utf8c-prev_kana_base)*3), 3));
	  	else
	  		utf8::unchecked::utf32to8(&prev_utf8c, &prev_utf8c + 1, back_inserter(romaji));
	  }
  	prev_utf8c = utf8c;
  	prev_kana_base = kana_base;

    utf8c = utf8::unchecked::next(it);
    if (utf8c > 12352 && utf8c < 12436)
    {
    	// Hiragana
    	kana_base = 12353;
    }
    else if (utf8c > 12448 && utf8c < 12535)
    {
    	// Katakana
    	kana_base = 12449;
    }
    else
    	kana_base = 0;

    if (kana_base && prev_utf8c == (prev_kana_base + 34))
    {	// Little tsu
    	romaji += no_spaces(string(kana_table+((utf8c-kana_base)*3), 1) +
    											string(kana_table+((utf8c-kana_base)*3), 3));
    	prev_utf8c = utf8c = 0;
    }
    else if (kana_base && utf8c == (kana_base + 66))
  	{	// Little ya
  		switch(prev_utf8c-prev_kana_base)
  		{
  			case 12: romaji += "kya";
  			break;
  			case 22: romaji += "sha";
  			break;
  			case 23: romaji += "ja";
  			break;
  			case 32: romaji += "cha";
  			break;
  			case 33: romaji += "ja";
  			break;
  			case 42: romaji += "nya";
  			break;
  			case 49: romaji += "hya";
  			break;
  			case 50: romaji += "bya";
  			break;
  			case 51: romaji += "pya";
  			break;
  			case 62: romaji += "mya";
  			break;
  			case 73: romaji += "rya";
  			break;
  			default: romaji += no_spaces(
  								string(kana_table+((prev_utf8c-prev_kana_base)*3), 3)) +
  								no_spaces(string(kana_table+((utf8c-kana_base)*3), 3));
  		}
  		prev_utf8c = utf8c = 0;
  	}
  	else if (kana_base && utf8c == (kana_base + 68))
  	{	// Little yu
  		switch(prev_utf8c-prev_kana_base)
  		{
  			case 12: romaji += "kyu";
  			break;
  			case 22: romaji += "shu";
  			break;
  			case 23: romaji += "ju";
  			break;
  			case 32: romaji += "chu";
  			break;
  			case 33: romaji += "ju";
  			break;
  			case 42: romaji += "nyu";
  			break;
  			case 49: romaji += "hyu";
  			break;
  			case 50: romaji += "byu";
  			break;
  			case 51: romaji += "pyu";
  			break;
  			case 62: romaji += "myu";
  			break;
  			case 73: romaji += "ryu";
  			break;
  			default: romaji += no_spaces(
  								string(kana_table+((prev_utf8c-prev_kana_base)*3), 3)) +
  								no_spaces(string(kana_table+((utf8c-kana_base)*3), 3));
  		}
  		prev_utf8c = utf8c = 0;
  	}
  	else if (kana_base && utf8c == (kana_base + 70))
  	{	// Little yo
  		switch(prev_utf8c-prev_kana_base)
  		{
  			case 12: romaji += "kyo";
  			break;
  			case 22: romaji += "sho";
  			break;
  			case 23: romaji += "jo";
  			break;
  			case 32: romaji += "cho";
  			break;
  			case 33: romaji += "jo";
  			break;
  			case 42: romaji += "nyo";
  			break;
  			case 49: romaji += "hyo";
  			break;
  			case 50: romaji += "byo";
  			break;
  			case 51: romaji += "pyo";
  			break;
  			case 62: romaji += "myo";
  			break;
  			case 73: romaji += "ryo";
  			break;
  			default: romaji += no_spaces(
  								string(kana_table+((prev_utf8c-prev_kana_base)*3), 3)) +
  								no_spaces(string(kana_table+((utf8c-kana_base)*3), 3));
  		}
  		prev_utf8c = utf8c = 0;
  	}
  }
  if (prev_utf8c)
  {
  	if (prev_kana_base)
	  	romaji += no_spaces(string(kana_table+((prev_utf8c-prev_kana_base)*3), 3));
	  else
	  	utf8::unchecked::utf32to8(&prev_utf8c, &prev_utf8c + 1, back_inserter(romaji));
	}
  if (utf8c)
  {
  	if (kana_base)
	  	romaji += no_spaces(string(kana_table+((utf8c-kana_base)*3), 3));
	  else
	  	utf8::unchecked::utf32to8(&utf8c, &utf8c + 1, back_inserter(romaji));
	}
  return romaji;
}
