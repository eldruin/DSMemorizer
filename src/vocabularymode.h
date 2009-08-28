/// \file vocabularymode.h Vocabulary memorizing game mode.
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

#ifndef VOCABULARYMODE_H_
#define VOCABULARYMODE_H_

class Card;
class TextBox;
class TextBoxHandler;
class XMLParser;

/// Vocabulary memorizing game mode.
/// Shows words with their reading and translation
class VocabularyMode
{
public:
  /// Default constructor
  VocabularyMode() {}
  /// \brief Initializer
  /// \param bgid Background id
  void Init (int bgid);

  /// Destructor
  ~VocabularyMode();

private:
  /// Print a card
  void PrintCard (const Card& card);
  /// Makes the next element of the card visible and prints it.
  /// If it's completely visible returns one more of the actual card number.
  int ViewNext(int card);

  /// XML database file parser
  XMLParser *xmlparser_;
  /// Text boxes handler
  TextBoxHandler* tbh_;
  TextBox *kanji_,                ///< Kanji
          *reading_,              ///< Reading
          *translation_,          ///< Translation
          *caption_kanji_,        ///< Kanji caption
          *caption_reading_,      ///< Reading caption
          *caption_translation_,  ///< Translation caption
          *card_number_;          ///< Card number
};

#endif // VOCABULARYMODE_H_

