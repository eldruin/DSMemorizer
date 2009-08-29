/// \file mainscreenhandler.h Handler for the main screen
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

#ifndef MAINSCREENHANDLER_H_
#define MAINSCREENHANDLER_H_

#include "types.h"

class Card;
class TextBox;
class ScreensHandler;

/// Handler for the main screen
class MainScreenHandler
{
public:
  /// Default constructor
  /// \param screen_mode Mode of the screen
  MainScreenHandler(Types::MainScreenMode::mode screen_mode);
  /// \brief Initializer
  /// \param screens_handler Already created and initialized screens handler
  void Init (ScreensHandler* screens_handler, int boxes_number = 0);
  /// Set the boxes captions
  void Captions (std::string box1, std::string box2, std::string box3);

  /// Print a card
  void PrintCard (const Card& card);
  /// Scrolls the screen
  void Scroll (int sx, int sy) const;
  /// Shows the next invisible box
  /// \return true if all was already visible
  bool ViewNext ();

  /// Destructor
  ~MainScreenHandler();

private:
  /// Base screens handler
  ScreensHandler* screens_handler_;
  /// Mode of the screen
  Types::MainScreenMode::mode screen_mode_;
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
          *box1_,                   ///< Box 1 text
          *box2_,                   ///< Box 2 text
          *box3_,                   ///< Box 3 text
          *caption_box1_,           ///< Box 1 caption text
          *caption_box2_,           ///< Box 2 caption text
          *caption_box3_;           ///< Box 3 caption text

  /// number of boxes [0-3]
  int boxes_number_;
  /// background image bitmap
  const unsigned int* bg_image_bitmap_;
  /// background image palette
  const unsigned short* bg_image_palette_;
  /// Background id
  int bgid_;
};

#endif // MAINSCREENHANDLER_H_

