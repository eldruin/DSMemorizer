/// \file types.h Type declarations.
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


#ifndef TYPES_H_
#define TYPES_H_

namespace Types
{
	enum Font
	{
		MONA_FONT,
		VERA_FONT
	};

	namespace Color
	{
		const short BLACK = 255;
		const short GREY = 254;
		const short WHITE = 253;
	}

  namespace MainScreenMode
  {
    enum mode
    {
      KANJI,
      VERTICAL_TEXTBOXES,
      VERTICAL_TEXTBOXES_VISIBLE
    };
  }

  namespace SubScreenMode
  {
    enum mode
    {
      CARDS,
      KANJI_CHOOSE
    };
  }

  namespace Screen
  {
    enum selector
    {
      MAIN,
      SUB
    };
  }
}

#endif // TYPES_H_

