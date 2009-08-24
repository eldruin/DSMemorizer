/// \file textbox.h Text box class.
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

#ifndef TEXTBOX_H_
#define TEXTBOX_H_

#include <ft2build.h>
#include FT_FREETYPE_H

/// Box of text with line control
class TextBox
{
public:
  /// Default constructor
  TextBox() {}
  /// Initializer
  /// \param bgid background id
  /// \param face FT_Face object from the TextHandler class
  /// \param size Font size
  /// \param x X position of the upper left corner
  /// \param y Y position of the upper left corner
  /// \param width Width of the box in pixels
  /// \param height Maximum height of the box. By default is 0 which means
  /// infinite
  void Init (int bgid, FT_Face face, int size, int x, int y, int width,
             int height = 0);

  /// Get the font size
  int size () const;
  /// Get the X position of the upper left corner
  int x () const;
  /// Get the Y position of the upper left corner
  int y () const;
  /// Get the width of the box in pixels
  int width () const;
  /// Get the height of the box in pixels
  int height () const;
  /// Get the text
  std::string text () const;
  /// Set the text
  void text (const std::string& str);

  /// Moves the text box to another position
  void Move (int x, int y);
  /// Adjust the text box vertically if the upper box overlaps
  void Adjust (int y);
  /// Prints the text box in its position
  void Print ();

private:
  /// Face of the font, not necesary to have the correct size
  FT_Face face_;
  /// String of text to print
  std::string text_;
  int size_,              ///< Font size
      x_,                 ///< X position of the upper left corner
      y_,                 ///< Y position of the upper left corner
      width_,             ///< Width of the box in pixels
      height_;            ///< Height of the box in pixels
  bool mutable_height_;   ///< Whether the height is mutable or not

  /// Video buffer pointer to print in
	u16* video_buffer_;
};

#endif // TEXTBOX_H_
