/// \file graphics.cpp Graphics functions.
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
#include "types.h"
#include "graphics.h"

using namespace Graphics;
using namespace Types;

static const u16 rmask = BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4);
static const u16 gmask = BIT(5) | BIT(6) | BIT(7) | BIT(8) | BIT(9);
static const u16 bmask = BIT(10) | BIT(11) | BIT(12) | BIT(13) | BIT(14);

inline u8 red_color (u16 color)
{
  return (color & rmask);
}

inline u8 green_color (u16 color)
{
  return (color & gmask) >> 5;
}

inline u8 blue_color (u16 color)
{
  return (color & bmask) >> 10;
}

u16 black_color_main;
u16 gray_color_main;
u16 black_color_sub;
u16 gray_color_sub;

/// Converts a color in [0-255] range to the black, grey and white
/// palette indexes
inline u16 convert_color(u8 incolor, Screen::selector screen)
{
	u16 outcolor;
	if (incolor > 180)
		outcolor = (screen == Screen::MAIN) ? black_color_main : black_color_sub;
	else if (incolor > 128)
		outcolor = (screen == Screen::MAIN) ? gray_color_main : gray_color_sub;
	else
		outcolor = Color::TRANSPARENT;
	return outcolor;
}

/**
 *  \param src_palette Source color palette.
 *  \param dst_palette Destination color palette.
 *  \param palette_size Size in bytes of the palette.
 *  \param factor Factor to multiply colors by.
 *  Value range is [0-256]. As lower it is, the more lighter/darker the
 *  colors become.
 *  \param trans if trans == LIGHTER the palette gets lighter.\n
 *  if trans == DARKER the palette gets darker.
 */
void Graphics::FactorizePalette (const u16* src_palette, u16* dst_palette,
                                 int palette_size, u8 factor,
                                 ColorTransformation::mode trans)
{
  palette_size >>= 1;
  if (trans == ColorTransformation::LIGHTER)
    for (int i = 0; i < palette_size; ++i)
    {
      u16 color = src_palette[i];
      dst_palette[i] = RGB15(red_color(color)   * factor >> 8,
                             green_color(color) * factor >> 8,
                             blue_color(color)  * factor >> 8);
    }
  else
    for (int i = 0; i < palette_size; ++i)
    {
      u16 color = src_palette[i];
      dst_palette[i] = RGB15(red_color(color)   * (255 - factor) >> 8,
                             green_color(color) * (255 - factor) >> 8,
                             blue_color(color)  * (255 - factor) >> 8);
    }
}

/**
 *  \param bitmap Bitmap of the image.
 *  \param palette Palette of the image.
 *  \param screen Screen selector.
 *  \param bgid Background where to print ID.
 *  \param effect Can be either SplashEffect::APPEAR or
 *  SplashEffect::DISSOLVE.\n
 *  Both values can be ORed together. In this case the bitmap will appear
 *  and then dissolve.
 */
void Graphics::SplashImage (const unsigned int* bitmap,
                            const unsigned short* palette,
                            Screen::selector screen, int bgid, u8 effect)
{
  unsigned short* dst_palette;
  dmaCopy(bitmap, bgGetGfxPtr(bgid), SCREEN_WIDTH*SCREEN_HEIGHT);
  if (screen == Screen::MAIN)
    dst_palette = BG_PALETTE;
  else
    dst_palette = BG_PALETTE_SUB;

  if (effect & SplashEffect::APPEAR)
    for (int i = 0; i < 256; i+=2)
    {
      u16 tmp_palette[256];
      FactorizePalette (palette, tmp_palette, 256*2, i,
                        ColorTransformation::LIGHTER);
      swiWaitForVBlank();
      dmaCopy(tmp_palette, dst_palette, 256*2);
    }
  if (effect & SplashEffect::DISSOLVE)
    for (int i = 0; i < 256; i+=2)
    {
      u16 tmp_palette[256];
      FactorizePalette (palette, tmp_palette, 256*2, i,
                        ColorTransformation::DARKER);
      swiWaitForVBlank();
      dmaCopy(tmp_palette, dst_palette, 256*2);
    }
}

/**
 *  \param x Upper left corner x coordinate where to print the bitmap.
 *  \param y Upper left corner y coordinate where to print the bitmap.
 *  \param width Width of the bitmap.
 *  \param height Height of the bitmap.
 *  \param bitmap Pointer to an array of 4 bytes words containing the bitmap
 *         pixels color palette index.
 *  \param key_color Masking color.
 *  \param bgid Background id where to print.
 *  \param screen Screen where to print in
 */
void Graphics::PrintBitmap (int x, int y, int width, int height,
                            const unsigned int* bitmap,
                            unsigned short key_color, int bgid,
                            Screen::selector screen)
{
  u16* video_buffer = bgGetGfxPtr(bgid);
  // Find the key color index in the palette not to be printed when found
  // in the bitmap.
  int key_color_index = FindColor (key_color, screen);

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
      // select the indicated byte in the word
      int pixel0 =
        ( (bitmap [(pixel_index_inverted >> 2)] &
          (0xFF000000 >> ((pixel_index_inverted%4)<<3))
        ) >> ((3-pixel_index_inverted%4) << 3 ));
      if (pixel0 == key_color_index) print_pixel0 = false;

      ++pixel_index_inverted;

      int pixel1 =
        ( (bitmap [(pixel_index_inverted >> 2)] &
          (0xFF000000 >> ((pixel_index_inverted%4)<<3))
        ) >> ((3-pixel_index_inverted%4) << 3 ));

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

/**
 *  \param dst_x Upper left corner x coordinate where to print the bitmap.
 *  \param dst_y Upper left corner y coordinate where to print the bitmap.
 *  \param region_x Upper left corner x coordinate where the region to print
 *         start in the bitmap.
 *  \param region_y Upper left corner y coordinate where the region to print
 *         start in the bitmap.
 *  \param region_width Width of the region to print.
 *  \param region_height Height of the region to print.
 *  \param bmp_width Width of the bitmap.
 *  \param bmp_height Height of the bitmap.
 *  \param bitmap Pointer to an array of 4 bytes words containing the source
 *         bitmap pixels color palette index.
 *  \param key_color Masking color.
 *  \param bgid Background id where to print.
 *  \param screen Screen where to print in
 */
void Graphics::PrintBitmapRegion (int dst_x, int dst_y, int region_x,
                                  int region_y, int region_width,
                                  int region_height, int bmp_width,
                                  int bmp_height,
                                  const unsigned int* bitmap,
                                  unsigned short key_color,
                                  int bgid,
                                  Screen::selector screen)
{
  u16* video_buffer = bgGetGfxPtr(bgid);
  // Find the key color index in the palette not to be printed when found
  // in the bitmap.
  int key_color_index = FindColor (key_color, screen);

  for (int image_y = 0; image_y < region_height; ++image_y)
  {
    int pixel_index_normal = (image_y+region_y) * bmp_width + region_x;
    for (int image_x = 0; image_x < region_width; image_x+=2, pixel_index_normal+=2)
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
      // select the indicated byte in the word
      int pixel0 =
        ( (bitmap [(pixel_index_inverted >> 2)] &
          (0xFF000000 >> ((pixel_index_inverted%4)<<3))
        ) >> ((3-pixel_index_inverted%4) << 3 ));
      if (pixel0 == key_color_index) print_pixel0 = false;

      ++pixel_index_inverted;

      int pixel1 =
        ( (bitmap [(pixel_index_inverted >> 2)] &
          (0xFF000000 >> ((pixel_index_inverted%4)<<3))
        ) >> ((3-pixel_index_inverted%4) << 3 ));

      if (pixel1 == key_color_index) print_pixel1 = false;

      int video_index = (image_y + dst_y)*128 + (image_x + dst_x)/2;

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
/**
 *  Function to print a 256-color bitmap of a font glyph converting colors.
 *  \param x Upper left corner x coordinate where to print the bitmap.
 *  \param y Upper left corner y coordinate where to print the bitmap.
 *  \param width Width of the bitmap.
 *  \param height Height of the bitmap.
 *  \param bitmap Pointer to an array of 4 bytes words containing the bitmap
 *         pixels color palette index.
 *  \param bgid Background id where to print.
 *  \param screen Screen where to print in
 */
void Graphics::PrintBitmap (int x, int y, int width, int height,
                            const unsigned char* bitmap, int bgid,
                            Screen::selector screen)
{
  u16* video_buffer = bgGetGfxPtr(bgid);

  for (int image_y = 0; image_y < height; ++image_y)
  {
    int image_x = 0;
    int index = image_y * width;
    for (; image_x < width-1; image_x += 2, index+=2)
    {
      bool print_pixel0 = true, print_pixel1 = true;
      u16 pixel0 = convert_color(bitmap[index], screen);
      print_pixel0 = (pixel0 != Color::TRANSPARENT);
      u16 pixel1 = convert_color(bitmap[index+1], screen);
      print_pixel1 = (pixel1 != Color::TRANSPARENT);
      int video_index = ((y + image_y) <<7) +
                         ((x + image_x) >> 1);

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
    if (image_x == width-1)
    {
      bool print_pixel = true;
      // for the last pixel in the row if the glyph width is odd
      int index = image_y * width + image_x;
      u16 pixel = convert_color(bitmap[index], screen);
      print_pixel = (pixel != Color::TRANSPARENT);
      int video_index = ((y + image_y) <<7) +
                         ((x + image_x) >> 1);
      if (print_pixel)
        // odd
        video_buffer[video_index] = pixel |
                                    (video_buffer[video_index] & 0xFF00);
    }
  }
}

void Graphics::Fill (unsigned short color, Screen::selector screen)
{
  if (screen == Screen::MAIN)
    dmaFillHalfWords(color, BG_PALETTE, 256*2);
  else
    dmaFillHalfWords(color, BG_PALETTE_SUB, 256*2);
}

unsigned short Graphics::FindColor (unsigned short color,
                                    Screen::selector screen,
                                    unsigned short offset)
{
  unsigned short i = offset;
  if (screen == Screen::MAIN)
    for (; i < 256 && BG_PALETTE[i] != color; ++i);
  else
    for (; i < 256 && BG_PALETTE_SUB[i] != color; ++i);
  return i;
}

void Graphics::SetColors ()
{
  black_color_main = FindColor (RGB15(0,0,0), Screen::MAIN);
  gray_color_main = FindColor (RGB15(15,15,15), Screen::MAIN);
  black_color_sub = FindColor (RGB15(0,0,0), Screen::SUB);
  gray_color_sub = FindColor (RGB15(15,15,15), Screen::SUB);
}
