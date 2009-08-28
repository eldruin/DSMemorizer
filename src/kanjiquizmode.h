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

class Card;
class TextBox;
class TextBoxHandler;
class XMLParser;

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

  /// Destructor
  ~KanjiQuizMode();

private:
  /// Prints the text in the two screens
  int PrintScreens (const Card& card);
  /// Prints a bitmap in a background
  static void PrintBitmap (int x, int y, int bgid, const unsigned int* bitmap,
                           int palette_offset);
  /// XML database file parser
  XMLParser *xmlparser_;
  /// Text boxes handler
  TextBoxHandler* tbh_;
  TextBox *translation_,              ///< Translation
          *on_reading_,               ///< On reading
          *kun_reading_,              ///< Kun reading
          *caption_translation_,      ///< Translation
          *caption_on_reading_,       ///< On reading
          *caption_kun_reading_,      ///< Kun reading
          *kanji1_,                   ///< Kanji 1
          *kanji2_,                   ///< Kanji 2
          *kanji3_,                   ///< Kanji 3
          *kanji4_,                   ///< Kanji 4
          *scoreboard_;               ///< Scoreboard
  /// Number of the correct kanji
  short correct_;
  /// Player score
  int score_;
};

#endif // KANJIQUIZMODE_H_

