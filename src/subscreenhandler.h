/// \file subscreenhandler.h Handler for the subscreen
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

#ifndef SUBSCREENHANDLER_H_
#define SUBSCREENHANDLER_H_

#include "types.h"

class Card;
class TextBox;
class ScreensHandler;

/// Handler for the subscreen
class SubScreenHandler
{
public:
  /// Constructor
  SubScreenHandler ();

  /// Set the screen mode
  void Init (Types::SubScreenMode::mode screen_mode,
             GameMode::mode game_mode,
             ScreensHandler* screens_handler);
  /// Switch mode
  void SwitchMode (Types::SubScreenMode::mode screen_mode,
                   GameMode::mode game_mode);

  /// Print a card
  void PrintCard (const Card& card);
  /// Print the score and accuracy
  void PrintBoards (int score, int accuracy);
  /// Print a tick
  void PrintTick (int position);
  /// Print a cross
  void PrintCross (int position);
  /// Kanjis text
  void PrintScreen (std::string kanji1, std::string kanji2,
                    std::string kanji3, std::string kanji4,
                    int score, int answers);
  /// Print the number of grades and strokes
  void PrintOptions(unsigned grade_min, unsigned grade_max,
                    unsigned strokes_min, unsigned strokes_max);
  /// Shows the background image of the screen in the current mode
  void DrawBgImage ();
  /// Fill the screen with a color
  void Fill (unsigned short color);

  /// Destructor
  ~SubScreenHandler();

private:
  /// Set the mode
  void SetMode (Types::SubScreenMode::mode screen_mode,
                GameMode::mode game_mode,
                ScreensHandler* screens_handler);
  /// Get actual mode bitmap image pointer
  const unsigned int* GetBitmapPtr ();
  /// Get actual mode palette image pointer
  const unsigned short* GetPalPtr ();
  /// Delete all members
  void ClearMembers ();
  /// Base screens handler
  ScreensHandler* screens_handler_;
  /// Screen mode
  Types::SubScreenMode::mode screen_mode_;
  TextBox *card_number_,      ///< Card number
          *scoreboard_,       ///< Scoreboard
          *accuracy_,         ///< Right answer percentage
          *box1_,             ///< Box 1
          *box2_,             ///< Box 2
          *box3_,             ///< Box 3
          *box4_,             ///< Box 4
          *mode_title_,       ///< Mode title
          *caption_grade_,    ///< Options grade text
          *caption_strokes_,  ///< Options strokes text
          *grade_min_,        ///< Options minimum grade box
          *grade_max_,        ///< Options maximum grade box
          *strokes_min_,      ///< Options minimum strokes box
          *strokes_max_;      ///< Options maximum strokes box

  unsigned prev_grade_min_,   ///< Previous grade minimum
           prev_grade_max_,   ///< Previous grade maximum
           prev_strokes_min_, ///< Previous strokes minimum
           prev_strokes_max_; ///< Previous strokes maximum

  /// Game mode
  GameMode::mode game_mode_;
  /// Background id
  int bgid_;
};

/// Calculates 4 random cards including the correct one given by card_index.
/// \return The correct card position
int randomize_positions (int card_index, int package_records, int& card0,
                         int& card1, int& card2, int& card3);

#endif // SUBSCREENHANDLER_H_
