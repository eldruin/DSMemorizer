/// \file files.h File paths.
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

#ifndef FILES_H_
#define FILES_H_

#include <string>
#include "types.h"

/// Files handling
namespace Files
{
  /// Returns the path of the font files
	std::string filepath (Types::Font f)
	{
		std::string path;
		switch(f)
		{
			case Types::MONA_FONT: path = std::string ("/fonts/mona.ttf");
				break;
      case Types::VERA_FONT: path = std::string ("/fonts/vera.ttf");
        break;
		}
		return path;
	}
}

#endif // FILES_H_

