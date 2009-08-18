/// \file kanjimodescreen.h Kanji mode screen manager.
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

#ifndef KANJIMODESCREEN_H_
#define KANJIMODESCREEN_H_

class Card;
class Text;

/// Screen manager for kanji mode
class KanjiModeScreen
{
public:
  /// Default constructor
  KanjiModeScreen() {}
  /// Initializer
  /// \param bgid Background id
  void Init (int bgid);
  /// Print a card
  void Print (const Card& card);

private:
  Text kanji_,                ///< Kanji
       first_reading_,        ///< First reading
       second_reading_,       ///< Second reading
       translation_,          ///< Translation
       example_kanji_,        ///< Example in kanjis
       example_reading_,      ///< Reading of the example
       example_translation_;  ///< Translation of the example

  /// Video buffer pointer to print in
  u16* video_buffer_;
};

#endif // KANJIMODESCREEN_H_

