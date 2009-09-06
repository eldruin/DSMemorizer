/// \file dsmemorizer.h Game main loop and main menu.
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

#ifndef DSMEMORIZER_H_
#define DSMEMORIZER_H_

class XMLParser;
class ScreensHandler;
class MainScreenHandler;
class SubScreenHandler;

/// Game main loop and main menu.
class DSMemorizer
{
public:
  /// Default constructor
  DSMemorizer() {};
  void Init();

private:
  /// Kanji game mode.
  /// Shows a kanji with its readings, translation and an example
  void KanjiMode ();

  /// Kanji quiz game mode.
  /// Shows the translation and the two readings of a kanji in the main screen
  /// and the right kanji plus 3 more random in the sub screen.
  /// The user have to guess which is the right one.
  void KanjiQuizMode ();

  /// Vocabulary memorizing game mode.
  /// Shows words with their reading and translation
  void VocabularyMode ();

  /// Vocabulary quiz game mode.
  /// Shows a word and its reading and asks for the kanji version.
  void VocabularyQuizMode ();

  XMLParser xmlparser_;
  ScreensHandler screens_handler_;
  MainScreenHandler main_screen_handler_;
  SubScreenHandler sub_screen_handler_;
};

#endif // DSMEMORIZER_H_

