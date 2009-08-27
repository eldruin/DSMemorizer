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

class Card;
class TextBox;
class TextBoxHandler;
class XMLParser;

/// Kanji game mode
class KanjiMode
{
public:
  /// Default constructor
  KanjiMode() {}
  /// \brief Initializer
  /// \param bgid Background id
  void Init (int bgid);

  /// Destructor
  ~KanjiMode();

private:
  /// Print a card
  void PrintCard (const Card& card);
  /// XML database file parser
  XMLParser *xmlparser_;
  /// Text boxes handler
  TextBoxHandler* tbh_;
  TextBox *kanji_,                  ///< Kanji
          *on_reading_,             ///< On reading
          *kun_reading_,            ///< Kun reading
          *translation_,            ///< Translation
          *example_kanji_,          ///< Example in kanjis
          *example_reading_,        ///< Reading of the example
          *example_translation_,    ///< Translation of the example
          *caption_kanji_,          ///< Kanji caption
          *caption_on_reading_,     ///< On reading caption
          *caption_kun_reading_,    ///< Kun reading caption
          *caption_translation_,    ///< Translation caption
          *caption_example_,        ///< Example caption
          *card_number_;            ///< Card number
};

#endif // KANJIMODESCREEN_H_

