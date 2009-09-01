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
#include "textbox.h"
#include "types.h"

using std::string;

/// Converts a color in [0-255] range to the black, grey and white
/// palette indexes
inline u8 convert_color(u8 incolor)
{
	u8 outcolor;
	if (incolor > 180)
		outcolor = Types::Color::BLACK;
	else if (incolor > 128)
		outcolor = Types::Color::GREY;
	else
		outcolor = Types::Color::WHITE;
	return outcolor;
}

void TextBox::Init (Types::Screen::selector screen, int bgid, FT_Face face,
                    int size, int x, int y, int width, int height)
{
  screen_ = screen;
  bgid_ = bgid;
  face_ = face;
  size_ = size;
  x_ = x;
  y_ = y;
  width_ = width;
  height_ = height;
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

void TextBox::text (const std::string& str)
{
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
  if (y > (y_ - ((size_*3)>>1)-5))
  {
    y_ = y + ((size_*3)>>1)+5;
  }
}

void TextBox::Print ()
{
  // set the size because it could have been changed by another textbox sharing
  // the same face
  FT_Set_Char_Size (face_, size_ << 6, 0, 110, 110);

  u16* video_buffer = bgGetGfxPtr(bgid_);

  int pen_x = x_;
  int pen_y = y_;
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
            height_ = pen_y;
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

      if (visible_)
      {
        // Added lots of local variables trying to speed up the rendering.
        int width = face_->glyph->bitmap.width;
        int rows = face_->glyph->bitmap.rows;
        int ycoord = pen_y - (face_->glyph->metrics.horiBearingY>>6);
        u8* buffer = face_->glyph->bitmap.buffer;

        pen_x += face_->glyph->metrics.horiBearingX>>6;
        // Copy the image from the rendered glyph bitmap to the video buffer
        for (int glyph_y = 0; glyph_y < rows; ++glyph_y)
        {
          int glyph_x = 0;
          int index = glyph_y * width;
          for (; glyph_x < width-1; glyph_x += 2, index+=2)
          {
            bool print_pixel0 = true, print_pixel1 = true;
            u8 pixel0 = convert_color(buffer[index]);
            if (pixel0 == Types::Color::WHITE) print_pixel0 = false;
            u8 pixel1 = convert_color(buffer[index+1]);
            if (pixel1 == Types::Color::WHITE) print_pixel1 = false;
            int video_index = ((ycoord + glyph_y) <<7) +
                               ((pen_x + glyph_x) >> 1);

            if (print_pixel0 && print_pixel1)
              video_buffer[video_index] = pixel0 | (pixel1 << 8);
            else if (print_pixel0)
              // odd
              video_buffer[video_index] = pixel0 |
                                          (video_buffer[video_index] & 0xFF00);

            else if (print_pixel1)
              // even
              video_buffer[video_index] = (pixel1 << 8) |
                                          (video_buffer[video_index] & 0x00FF);
          }
          if (glyph_x == width-1)
          {
            bool print_pixel = true;
            // for the last pixel in the row if the glyph width is odd
            int index = glyph_y * width + glyph_x;
            u8 pixel = convert_color(buffer[index]);
            if (pixel == Types::Color::WHITE) print_pixel = false;
            int video_index = ((ycoord + glyph_y) <<7) +
                               ((pen_x + glyph_x) >> 1);
            if (print_pixel)
              // odd
              video_buffer[video_index] = pixel |
                                          (video_buffer[video_index] & 0xFF00);
          }
        }
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
            height_ = pen_y;
          else
            return; // ERROR can't print a line more
        }
      }
    }
  }
}


