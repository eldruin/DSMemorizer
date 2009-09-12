/// \file textboxhandler.h Text boxes handler.
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

#ifndef TEXTBOXHANDLER_H_
#define TEXTBOXHANDLER_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include "types.h"

/// Text boxes handler class. Uses the FreeType2 library.
class TextBoxHandler
{
public:
	/// Default constructor
	TextBoxHandler() {}
	/// Initializer
	void Init();

  /// Creates a new text box in the class
  /// \param screen Screen where the text box is
  /// \param bgid background id
  /// \param font Font name
  /// \param size Font size
  /// \param x X position of the upper left corner
  /// \param y Y position of the upper left corner
  /// \param width Width of the box in pixels
  /// \param height Maximum height of the box. By default is 0 which means
  /// infinite
  /// \return Created TextBox ID
  TextBox* NewTextBox (Types::Screen::selector screen, int bgid,
                       Types::Font font, int size, int x, int y,
                       int width = 0, int height = 0);

  /// Destroy a text box
  void DestroyTextBox (TextBox* tb);
  /// Print all the text boxes
  void PrintAll (Types::Screen::selector);
  /// Destroyer
  ~TextBoxHandler();

private:
	/// Font library
	FT_Library library_;

  /// Font loaded
	Types::Font font_;
	/// Faces loaded cache
	std::vector< FT_Face > faces_;
  /// Created text boxes pointers
	std::vector< TextBox* > text_boxes_;
};

#endif // TEXTBOXHANDLER_H_


