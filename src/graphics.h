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

  /// Prints a bitmap in a background
  void PrintBitmap (int x, int y, int width, int height,
                  const unsigned int* bitmap, short key_color,
                  int palette_offset, int bgid, Types::Screen::selector screen);

  /// Fill the screen with a color
  void Fill (unsigned short color, Types::Screen::selector screen);
}

#endif // GRAPHICS_H_
