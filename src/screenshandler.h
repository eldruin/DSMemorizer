/// \file screenshandler.h Screen handler
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

#ifndef SCREENHANDLER_H_
#define SCREENHANDLER_H_

class TextBoxHandler;

/// Screens handler
class ScreensHandler
{
public:
  /// Default constructor
  ScreensHandler() {}

  /// \brief Initializer
  void Init ();

  /// Get the text box handler pointer
  TextBoxHandler* tbh () const;

  /// Destructor
  ~ScreensHandler();

protected:
  /// Text boxes handler
  TextBoxHandler* tbh_;
};

#endif // SCREENHANDLER_H_



