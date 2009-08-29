/// \file kanjimode.h Kanji game mode.
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

#ifndef KANJIMODE_H_
#define KANJIMODE_H_

class MainScreenHandler;
class SubScreenHandler;

/// Kanji game mode
class KanjiMode
{
public:
  /// Default constructor
  KanjiMode() {}
  /// \brief Initializer
  /// \param bgid Background id
  void Init (int bgid);

private:
  MainScreenHandler *main_screen_handler_;
  SubScreenHandler *sub_screen_handler_;
};

#endif // KANJIMODE_H_

