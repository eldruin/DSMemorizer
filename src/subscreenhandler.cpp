/// \file subscreenhandler.cpp Handler for the subscreen
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

#include <nds.h>
#include "card.h"
#include "textbox.h"
#include "textboxhandler.h"
#include "screenshandler.h"
#include "subscreenhandler.h"

#include "kanjisubbg.h"
#include "kanjiquizsubbg.h"
#include "vertical_textboxes_choose_bg.h"
#include "tick.h"
#include "cross.h"

using namespace Types;

/// Returns the position for the tick or cross on the kanji number 'position'
inline int x_position (int position)
{
  int x_pos = 0;
  switch (position)
  {
    case 1: x_pos = 43;
    break;
    case 2: x_pos = 98;
    break;
    case 3: x_pos = 153;
    break;
    case 4: x_pos = 200;
    break;
  }
  return x_pos;
}

/// Returns the position for the tick or cross on the kanji word
/// number 'position'
inline int y_position (int position)
{
  int y_pos = 0;
  switch (position)
  {
    case 1: y_pos = 39;
    break;
    case 2: y_pos = 68;
    break;
    case 3: y_pos = 97;
    break;
    case 4: y_pos = 126;
    break;
  }
  return y_pos;
}

/// Find a color in the palette of the sub screen starting on a given offset
inline int find_color_in_palette_sub(short color, int palette_offset)
{
  int i = palette_offset;
  for (; i < 256 && BG_PALETTE_SUB[i] != color; ++i);
  return i;
}

SubScreenHandler::SubScreenHandler(Types::SubScreenMode::mode screen_mode)
{
  screen_mode_ = screen_mode;
  if (screen_mode_ == SubScreenMode::CARDS)
  {
    bg_image_bitmap_ = kanjisubbgBitmap;
    bg_image_palette_ = kanjisubbgPal;
  }
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
  {
    bg_image_bitmap_ = kanjiquizsubbgBitmap;
    bg_image_palette_ = kanjiquizsubbgPal;
  }
  if (screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
  {
    bg_image_bitmap_ = vertical_textboxes_choose_bgBitmap;
    bg_image_palette_ = vertical_textboxes_choose_bgPal;
  }
}

void SubScreenHandler::Init (ScreensHandler* screens_handler)
{
  screens_handler_ = screens_handler;

  videoSetModeSub(MODE_5_2D);
  vramSetBankC(VRAM_C_SUB_BG);

  bgid_ = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

  dmaCopy(bg_image_palette_, BG_PALETTE_SUB, 256*2);

  short white_color = 0;
  if (screen_mode_ == SubScreenMode::CARDS)
  {
    card_number_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::VERA_FONT, 8,70,20,225,0);
    card_number_->floats(true);
    white_color = RGB15(18,18,28);
  }
  else if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
  {
    dmaCopy(tickPal,BG_PALETTE_SUB+8,43*2);
    dmaCopy(crossPal,BG_PALETTE_SUB+94,43*2);
    white_color = RGB15(28,28,20);

    scoreboard_  =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::VERA_FONT, 8,154,25,225,0);
    accuracy_  =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::VERA_FONT, 8,23,25,225,0);
    box1_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,33,80,225,0);
    box2_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,85,80,225,0);
    box3_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,137,80,225,0);
    box4_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,189,80,225,0);
    scoreboard_->floats(true);
    accuracy_->floats(true);
    box1_->floats(true);
    box2_->floats(true);
    box3_->floats(true);
    box4_->floats(true);
  }
  else if (screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
  {
    dmaCopy(tickPal,BG_PALETTE_SUB+8,43*2);
    dmaCopy(crossPal,BG_PALETTE_SUB+94,43*2);
    white_color = RGB15(28,28,20);

    scoreboard_  =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::VERA_FONT, 8,154,25,225,0);
    accuracy_  =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::VERA_FONT, 8,23,25,225,0);
    box1_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 10,33,54,225,0);
    box2_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 10,33,83,225,0);
    box3_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 10,33,112,225,0);
    box4_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 10,33,141,225,0);
    scoreboard_->floats(true);
    accuracy_->floats(true);
    box1_->floats(true);
    box2_->floats(true);
    box3_->floats(true);
    box4_->floats(true);
  }

  BG_PALETTE_SUB[Types::Color::BLACK] = RGB15(0,0,0);
  BG_PALETTE_SUB[Types::Color::GREY] = RGB15(15,15,15);
  BG_PALETTE_SUB[Types::Color::WHITE] = white_color;
}

void SubScreenHandler::PrintCard (const Card& card)
{
  dmaCopy(bg_image_bitmap_, bgGetGfxPtr(bgid_), 256*256);
 	if (screen_mode_ == SubScreenMode::CARDS)
  {
    char* card_number_text = new char [40];
    sprintf(card_number_text, "Card number: %i",card.index());
    card_number_->text(card_number_text);
    delete card_number_text;
    card_number_->Print();
  }
}

void SubScreenHandler::PrintBoards (int score, int answers)
{
  dmaCopy(bg_image_bitmap_, bgGetGfxPtr(bgid_), 256*33);
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
  {
    char* score_text = new char [40];
    sprintf(score_text, "Score: %i",score);
    scoreboard_->text(score_text);
    delete score_text;

    scoreboard_->Print();

    char* accuracy_text = new char [40];
    sprintf(accuracy_text, "Accuracy: %i%%",score*100/answers);
    accuracy_->text(accuracy_text);
    delete accuracy_text;

    accuracy_->Print();
  }
}

void SubScreenHandler::PrintTick (int position)
{
  int x_pos = 0, y_pos = 0;
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
    x_pos = x_position(position), y_pos = 103;
  else if (screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
    x_pos = 10 , y_pos = y_position(position);

  PrintBitmap(x_pos, y_pos, 20, 20, bgid_, tickBitmap,RGB15(18,18,28),8);
}

void SubScreenHandler::PrintCross (int position)
{
  int x_pos = 0, y_pos = 0;
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
    x_pos = x_position(position), y_pos = 103;
  else if (screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
    x_pos = 10 , y_pos = y_position(position);

  PrintBitmap(x_pos, y_pos, 20, 20, bgid_, crossBitmap, RGB15(18,18,28),94);
}

void SubScreenHandler::PrintScreen (std::string kanji1, std::string kanji2,
                                    std::string kanji3, std::string kanji4,
                                    int score, int answers)
{
  dmaCopy(bg_image_bitmap_, bgGetGfxPtr(bgid_), 256*256);
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE ||
      screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
  {
    box1_->text(kanji1);
    box2_->text(kanji2);
    box3_->text(kanji3);
    box4_->text(kanji4);

    char* score_text = new char [40];
    sprintf(score_text, "Score: %i",score);
    scoreboard_->text(score_text);
    delete score_text;

    scoreboard_->Print();

    char* accuracy_text = new char [40];
    sprintf(accuracy_text, "Accuracy: %i%%",score*100/answers);
    accuracy_->text(accuracy_text);
    delete accuracy_text;

    screens_handler_->tbh()->PrintAll(Screen::SUB);
  }
}

/**
 *  \param x Upper left corner x coordinate where to print the bitmap.
 *  \param y Upper left corner y coordinate where to print the bitmap.
 *  \param width Width of the bitmap.
 *  \param height Height of the bitmap.
 *  \param bgid Background id where to print.
 *  \param bitmap Pointer to an array of 4 bytes words containing the bitmap
 *         pixels color palette index.
 *  \param key_color Masking color.
 *  \param palette_offset Offset to the color palette where the colors of
 *         the bitmap start.
 */
  void SubScreenHandler::PrintBitmap (int x, int y, int width, int height,
                                      int bgid, const unsigned int* bitmap,
                                      short key_color, int palette_offset)
{
  u16* video_buffer = bgGetGfxPtr(bgid);
  int key_color_index = find_color_in_palette_sub(key_color, palette_offset);
  for (int image_y = 0; image_y < height; ++image_y)
    for (int image_x = 0; image_x < width; image_x+=2)
    {
      bool print_pixel0 = true, print_pixel1 = true;
      int pixel_index_normal = image_y * width + image_x;
      // Now convert the bitmap index because of the half words inversion
      // in the words of the bitmap.
      // Conversion is done like this:
      // -----------------          old  new
      // | a | b | c | d |          0  -> 2
      // -----------------          1  -> 3
      //         |                  2  -> 0
      //         v                  3  -> 1
      // -----------------
      // | c | d | a | b |
      // -----------------
      int pixel_index_inverted = pixel_index_normal -
        (pixel_index_normal % 4) + ((pixel_index_normal % 4 + 2) & 3);
      // select the indicated byte in the word and adds the offset
      // for the new palette
      int pixel0 =
        ( (bitmap [(pixel_index_inverted >> 2)] &
          (0xFF000000 >> ((pixel_index_inverted%4)<<3))
        ) >> ((3-pixel_index_inverted%4) << 3 )) + palette_offset;
      if (pixel0 == key_color_index) print_pixel0 = false;

      ++pixel_index_inverted;

      int pixel1 =
        ( (bitmap [(pixel_index_inverted >> 2)] &
          (0xFF000000 >> ((pixel_index_inverted%4)<<3))
        ) >> ((3-pixel_index_inverted%4) << 3 )) + palette_offset;

      if (pixel1 == key_color_index) print_pixel1 = false;

      int video_index = (image_y + y)*128 + (image_x + x)/2;

      if (print_pixel0)
        // even
        video_buffer[video_index] = (pixel0 << 8) |
                                    (video_buffer[video_index] & 0x00FF);

      if (print_pixel1)
        // odd
        video_buffer[video_index] = pixel1 |
                                    (video_buffer[video_index] & 0xFF00);
    }
}

SubScreenHandler::~SubScreenHandler()
{
  if (screen_mode_ == SubScreenMode::CARDS)
  {
    screens_handler_->tbh()->DestroyTextBox(card_number_);
  }
  else if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
  {
    screens_handler_->tbh()->DestroyTextBox(scoreboard_);
    screens_handler_->tbh()->DestroyTextBox(accuracy_);
    screens_handler_->tbh()->DestroyTextBox(box1_);
    screens_handler_->tbh()->DestroyTextBox(box2_);
    screens_handler_->tbh()->DestroyTextBox(box3_);
    screens_handler_->tbh()->DestroyTextBox(box4_);
  }
}


int randomize_positions (int card_index, int package_records, int& card0,
                         int& card1, int& card2, int& card3)
{

  int selected_card [4] = {0,0,0,0};
  selected_card[0] = card_index;
  // Fills the vector with not repeated random cards
  for (size_t i = 1; i < 4; ++i)
  {
    bool chosen;
    do
    {
      chosen = false;
      selected_card[i] = rand()%package_records + 1;
      for (size_t j = 0;!chosen && j < 4; ++j)
        chosen = (i!=j) && (selected_card[j] == selected_card[i]);
    }while(chosen);
  }

  // randomizes the correct card position
  int new_position = rand()%4;
  int aux = selected_card[new_position];
  selected_card[new_position] = selected_card[0];
  card0 = aux;
  card1 = selected_card[1];
  card2 = selected_card[2];
  card3 = selected_card[3];

  return new_position + 1;
}

