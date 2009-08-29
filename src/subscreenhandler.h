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
  /// Default constructor
  SubScreenHandler (Types::SubScreenMode::mode screen_mode);
  /// \brief Initializer
  /// \param sub_screen_mode Sub screen mode
  /// \param tbh Already initialized TextBoxHandler
  void Init (ScreensHandler* screens_handler);

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

  /// Destructor
  ~SubScreenHandler();

private:
  /// Prints a bitmap in a background
  static void PrintBitmap (int x, int y, int bgid, const unsigned int* bitmap,
                           int palette_offset);

  /// Base screens handler
  ScreensHandler* screens_handler_;
  /// Screen mode
  Types::SubScreenMode::mode screen_mode_;
  TextBox *card_number_,      ///< Card number
          *scoreboard_,       ///< Scoreboard
          *accuracy_,         ///< Right answer percentage
          *kanji1_,           ///< Kanji 1
          *kanji2_,           ///< Kanji 2
          *kanji3_,           ///< Kanji 3
          *kanji4_;           ///< Kanji 4
  /// background image bitmap
  const unsigned int* bg_image_bitmap_;
  /// background image palette
  const unsigned short* bg_image_palette_;
  /// Background id
  int bgid_;
};

#endif // SUBSCREENHANDLER_H_
