/// \file types.h Type declarations.
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


#ifndef TYPES_H_
#define TYPES_H_

/// Types of things
namespace Types
{
  /// Font types
	enum Font
	{
		MONA_FONT,
		VERA_FONT
	};

  /// Available modes for the main screen
  namespace MainScreenMode
  {
    /// Mode
    enum mode
    {
      KANJI,                      ///< Kanjis main screen
      VERTICAL_TEXTBOXES,         ///< Screen filled with text boxes
                                  /// vertically. Only the first box text is
                                  /// visible.
      VERTICAL_TEXTBOXES_VISIBLE, ///< Screen filled with text boxes
                                  /// vertically. All boxes are visible.
      SPLASH_SCREEN               ///< Show the splash screen.
    };
  }

  /// Available modes for the sub screen
  namespace SubScreenMode
  {
    /// Mode
    enum mode
    {
      CARDS,                        ///< Screen showing the card number and two
                                    /// cards to navigate.
      KANJI_CHOOSE,                 ///< Screen showing 4 kanjis to choose and
                                    /// the score and accuracy percentage
      VERTICAL_TEXTBOXES_CHOOSE,    ///< Vertical textboxes
      MAIN_MENU,                    ///< Main menu
      OPTIONS_GRADE_STROKES_ROMAJI, ///< Grade, strokes and romaji select
                                    /// options mode
      OPTIONS_ROMAJI                ///< Romaji conversion option mode
    };
  }

  /// Screens
  namespace Screen
  {
    /// Screen selector
    enum selector
    {
      MAIN,   ///< Main screen
      SUB     ///< Sub screen
    };
  }
}

/// Game modes
namespace GameMode
{
  /// Game modes
  enum mode
  {
    NONE,
    KANJI,
    KANJI_QUIZ,
    VOCABULARY,
    VOCABULARY_QUIZ,
    MAIN_MENU,
    SPLASH_SCREEN
  };
}

#endif // TYPES_H_

