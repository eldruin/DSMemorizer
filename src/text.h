/// \file text.h Text printing.
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

#ifndef TEXT_H_
#define TEXT_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include "types.h"

/// Text rendering class. Uses the FreeType2 library.
class Text
{
public:
	/// Default constructor
	Text() {}
	/// Initializer
	/// \param bgid background id
	void Init(int bgid);

	/// Load a font with a size
	/// \param f Font name
	/// \param size Font size
	void LoadFace(Types::Font f, int size);

	/// Prints a string in a position (no line changing support).
	/// \param string UTF-8 string.
	/// \param x X coordinate where to print, it's updated when returns.
	/// \param y Y coordinate where to print. It's not updated because
	/// this function prints from left to right and don't support
	/// changing the line.
	void Print(const std::string& str, int& x, int& y);

	void UnloadFace(Types::Font f, int size);
private:
	// Needs cache system
	/// Font library
	FT_Library library_;
	/// Font loaded face
	FT_Face face_;
	/// Font size
	int size_;
	/// Video buffer pointer to print in
	u16* video_buffer_;
};

#endif // TEXT_H_

