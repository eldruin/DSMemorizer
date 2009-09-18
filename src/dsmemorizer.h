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

class SoundHandler;
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
  /// Init and start the game
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

  /// Set the options of grade, strokes and romaji conversion
  void SetOptions(unsigned& grade_min, unsigned& grade_max,
                  unsigned& strokes_min, unsigned& strokes_max);
  /// Set the option of the romaji conversion
  void SetOptions ();
  /// Romaji conversion option
  bool romaji_conversion;

  /// Sound handler
  SoundHandler sound_handler_;
  /// XML files parser.
  XMLParser xmlparser_;
  /// Handler for screen common things.
  ScreensHandler screens_handler_;
  /// Main screen handler.
  MainScreenHandler main_screen_handler_;
  /// Sub screen handler.
  SubScreenHandler sub_screen_handler_;
};

#endif // DSMEMORIZER_H_

