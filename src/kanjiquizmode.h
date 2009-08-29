/// \file kanjiquizmode.h Kanji quiz game mode.
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

#ifndef KANJIQUIZMODE_H_
#define KANJIQUIZMODE_H_

/// Kanji quiz game mode.
/// Shows the translation and the two readings of a kanji in the main screen
/// and the right kanji plus 3 more random in the sub screen.
/// The user have to guess which is the right one.
class KanjiQuizMode
{
public:
  /// Default constructor
  KanjiQuizMode() {}
  /// \brief Initializer
  /// \param bgid Background id
  void Init (int bgid);

private:
  /// Number of the correct kanji
  short correct_;
  /// Player score
  int score_;
  /// Player answer number
  int answers_;
};

#endif // KANJIQUIZMODE_H_

