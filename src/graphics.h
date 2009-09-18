/// \file graphics.h Graphics functions.
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

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "types.h"

/// Graphics functions
namespace Graphics
{
  /// Color transformations.
  namespace ColorTransformation
  {
    /// Color transformation for the function Graphics::factorize_palette.
    enum mode
    {
      LIGHTER,  ///< Transformation to lighter.
      DARKER    ///< Transformation to darker.
    };
  }
  /// Multiply a palette by a certain factor making colors lighter or darker
  void FactorizePalette (const u16* src_palette, u16* dst_palette,
                          int palette_size, u8 factor,
                          ColorTransformation::mode trans);

  /// Splash available effects.
  namespace SplashEffect
  {
    const char APPEAR = 1;    ///< Appear from darker to lighter.
    const char DISSOLVE = 2;  ///< Dissolve from lighter to darker.
  }

  /// Splash an image in a screen.
  void SplashImage (const unsigned int* bitmap, const unsigned short* palette,
                    Types::Screen::selector screen, int bgid, u8 effect);

  /// Prints a bitmap from grit in a background
  void PrintBitmap (int x, int y, int width, int height,
                    const unsigned int* bitmap, unsigned short key_color,
                    int bgid, Types::Screen::selector screen);

  /// Print a region of a bitmap in a background
  void PrintBitmapRegion (int dst_x, int dst_y, int region_x,
                          int region_y, int region_width,
                          int region_height, int bmp_width, int bmp_height,
                          const unsigned int* bitmap,
                          unsigned short key_color, int bgid,
                          Types::Screen::selector screen,
                          bool use_key_color = true);

  /// Redraw a region of a background
  void RedrawBgRegion (int x, int y, int region_width, int region_height,
                       Types::MainScreenMode::mode screen_mode,
                       GameMode::mode game_mode, int bgid);

  /// Redraw a region of a background
  void RedrawBgRegion (int x, int y, int region_width, int region_height,
                       Types::SubScreenMode::mode screen_mode,
                       GameMode::mode game_mode, int bgid);

  /// Print a bitmap in a background
  void PrintBitmap (int x, int y, int width, int height,
                    const unsigned char* bitmap, int bgid,
                    Types::Screen::selector screen);

  /// Fill the screen with a color
  void Fill (unsigned short color, Types::Screen::selector screen);

  /// Find a color in the palette of the given screen.
  /// Optional offset to the start of the palette can be given.
  unsigned short FindColor (unsigned short color,
                            Types::Screen::selector screen,
                            unsigned short offset = 0);
  /// Set black and gray colors variables to render fonts
  void SetColors ();

  /// Get the direction of the background bitmap for the screen mode for the
  /// main screen.
  const unsigned int* GetBitmapPtr (Types::MainScreenMode::mode screen_mode);
  /// Get the direction of the background palette for the screen mode for the
  /// main screen.
  const unsigned short* GetPalPtr (Types::MainScreenMode::mode screen_mode);
  /// Get the direction of the background bitmap for the screen mode for the
  /// sub screen.
  const unsigned int* GetBitmapPtr (Types::SubScreenMode::mode screen_mode,
                                    GameMode::mode game_mode);
  /// Get the direction of the background palette for the screen mode for the
  /// sub screen.
  const unsigned short* GetPalPtr (Types::SubScreenMode::mode screen_mode,
                                   GameMode::mode game_mode);
}

#endif // GRAPHICS_H_
