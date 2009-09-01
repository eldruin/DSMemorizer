/// \file screenshandler.cpp Screen handler
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
#include "textbox.h"
#include "textboxhandler.h"
#include "screenshandler.h"

using namespace Types;

void ScreensHandler::Init()
{
  tbh_ = new TextBoxHandler ();
  tbh_->Init ();
}

TextBoxHandler* ScreensHandler::tbh () const
{
  return tbh_;
}

ScreensHandler::~ScreensHandler ()
{
  tbh_->~TextBoxHandler();
  delete tbh_;
}

/**
 *  \param x Upper left corner x coordinate where to print the bitmap.
 *  \param y Upper left corner y coordinate where to print the bitmap.
 *  \param width Width of the bitmap.
 *  \param height Height of the bitmap.
 *  \param bitmap Pointer to an array of 4 bytes words containing the bitmap
 *         pixels color palette index.
 *  \param key_color Masking color.
 *  \param palette_offset Offset to the color palette where the colors of
 *         the bitmap start.
 *  \param bgid Background id where to print.
 *  \param screen Screen where to print in
 */
void PrintBitmap (int x, int y, int width, int height,
                  const unsigned int* bitmap, short key_color,
                  int palette_offset, int bgid, Screen::selector screen)
{
  u16* video_buffer = bgGetGfxPtr(bgid);
  // Find the key color index in the palette not to be printed when found
  // in the bitmap.
  int i = palette_offset;
  if (screen == Screen::MAIN)
    for (; i < 256 && BG_PALETTE[i] != key_color; ++i);
  else
    for (; i < 256 && BG_PALETTE_SUB[i] != key_color; ++i);
  int key_color_index = i;

  for (int image_y = 0; image_y < height; ++image_y)
  {
    int pixel_index_normal = image_y * width;
    for (int image_x = 0; image_x < width; image_x+=2, pixel_index_normal+=2)
    {
      bool print_pixel0 = true, print_pixel1 = true;
      // Now convert the bitmap index because of the half words inversion
      // in the words of the bitmap.
      // Conversion is done like this:
      // -----------------          old  new
      // | a | b | c | d |          0  -> 2
      // -----------------          1  -> 3
      //         |                  2  -> 0
      //         v                  3  -> 1
      // -----------------
      // | c | d | a | b |
      // -----------------
      int pixel_index_inverted = pixel_index_normal -
        (pixel_index_normal % 4) + ((pixel_index_normal % 4 + 2) & 3);
      // select the indicated byte in the word and adds the offset
      // for the new palette
      int pixel0 =
        ( (bitmap [(pixel_index_inverted >> 2)] &
          (0xFF000000 >> ((pixel_index_inverted%4)<<3))
        ) >> ((3-pixel_index_inverted%4) << 3 )) + palette_offset;
      if (pixel0 == key_color_index) print_pixel0 = false;

      ++pixel_index_inverted;

      int pixel1 =
        ( (bitmap [(pixel_index_inverted >> 2)] &
          (0xFF000000 >> ((pixel_index_inverted%4)<<3))
        ) >> ((3-pixel_index_inverted%4) << 3 )) + palette_offset;

      if (pixel1 == key_color_index) print_pixel1 = false;

      int video_index = (image_y + y)*128 + (image_x + x)/2;

      if (print_pixel0)
        // even
        video_buffer[video_index] = (pixel0 << 8) |
                                    (video_buffer[video_index] & 0x00FF);

      if (print_pixel1)
        // odd
        video_buffer[video_index] = pixel1 |
                                    (video_buffer[video_index] & 0xFF00);
    }
  }
}
