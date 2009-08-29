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
#include "tick.h"
#include "cross.h"

using namespace Types;

/// Returns the position of the textbox to show the kanji number 'position'
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
    kanji1_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,33,80,225,0);
    kanji2_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,85,80,225,0);
    kanji3_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,137,80,225,0);
    kanji4_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,189,80,225,0);
    scoreboard_->floats(true);
    accuracy_->floats(true);
    kanji1_->floats(true);
    kanji2_->floats(true);
    kanji3_->floats(true);
    kanji4_->floats(true);
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
  PrintBitmap(x_position(position), 103, bgid_, tickBitmap,8);
}

void SubScreenHandler::PrintCross (int position)
{
  PrintBitmap(x_position(position), 103, bgid_, crossBitmap,94);
}

void SubScreenHandler::PrintScreen (std::string kanji1, std::string kanji2,
                                    std::string kanji3, std::string kanji4,
                                    int score, int answers)
{
  dmaCopy(bg_image_bitmap_, bgGetGfxPtr(bgid_), 256*256);
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
  {
    kanji1_->text(kanji1);
    kanji2_->text(kanji2);
    kanji3_->text(kanji3);
    kanji4_->text(kanji4);

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

void SubScreenHandler::PrintBitmap (int x, int y, int bgid,
                                    const unsigned int* bitmap,
                                    int palette_offset)
{
  // FIXME: Don't print correctly
  u16* video_buffer = bgGetGfxPtr(bgid);
  for (int image_y = 0; image_y < 20; ++image_y)
    for (int image_x = 0; image_x < 20; ++image_x)
    {
      int pixel_num = image_y * 20 + image_x;
      // select the indicated byte in the word and adds the offset
      // for the new palette
      int pixel_palette_index =
        ((bitmap [(pixel_num >> 2)] & (0xFF000000 >> (pixel_num % 4))
        ) >> ((3-pixel_num%4) << 3 )) + palette_offset;
      int video_index = (image_y + y)*128 + (image_x + x)/2;
      if ((image_x+x) % 2)
      {
        // odd
        video_buffer[video_index] = pixel_palette_index |
                                    (video_buffer[video_index] & 0xFF00);
      }
      else
      {
        // even
        video_buffer[video_index] = (pixel_palette_index << 8) |
                                    (video_buffer[video_index] & 0x00FF);
      }
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
    screens_handler_->tbh()->DestroyTextBox(kanji1_);
    screens_handler_->tbh()->DestroyTextBox(kanji2_);
    screens_handler_->tbh()->DestroyTextBox(kanji3_);
    screens_handler_->tbh()->DestroyTextBox(kanji4_);
  }
}
