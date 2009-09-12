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
#include "graphics.h"

#include "kanjisubbg.h"
#include "kanjiquizsubbg.h"
#include "vertical_textboxes_choose_bg.h"
#include "main_menu_bg.h"

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

SubScreenHandler::SubScreenHandler()
{
  videoSetModeSub(MODE_5_2D);
  vramSetBankC(VRAM_C_SUB_BG);

  bgid_ = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
  // To check later if it's set mode or switch mode
  screens_handler_ = NULL;
}

/**
 *  \param screen_mode Mode of the screen
 *  \param screens_handler Already created and initialized screens handler
 */
void SubScreenHandler::Init (SubScreenMode::mode screen_mode,
                             ScreensHandler* screens_handler)
{
  SetMode (screen_mode, screens_handler);
}

/// \param screen_mode Mode of the screen
void SubScreenHandler::SwitchMode (SubScreenMode::mode screen_mode)
{
  ClearMembers ();
  SetMode (screen_mode, screens_handler_);
}

/**
 *  \param screen_mode Mode of the screen
 *  \param screens_handler Already created and initialized screens handler
 */
void SubScreenHandler::SetMode (SubScreenMode::mode screen_mode,
                                ScreensHandler* screens_handler)
{
  screen_mode_ = screen_mode;
  screens_handler_ = screens_handler;

  if (screen_mode_ == SubScreenMode::CARDS)
  {
    card_number_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::VERA_FONT, 8,70,8);
    card_number_->floats(true);
  }
  else if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
  {
    scoreboard_  =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::VERA_FONT, 8,154,13);
    accuracy_  =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::VERA_FONT, 8,23,13);
    box1_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,33,35);
    box2_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,85,35);
    box3_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,137,35);
    box4_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 30,189,35);
    scoreboard_->floats(true);
    accuracy_->floats(true);
    box1_->floats(true);
    box2_->floats(true);
    box3_->floats(true);
    box4_->floats(true);
  }
  else if (screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
  {
    scoreboard_  =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::VERA_FONT, 8,154,13);
    accuracy_  =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::VERA_FONT, 8,23,13);
    box1_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 10,33,39);
    box2_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 10,33,68);
    box3_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 10,33,97);
    box4_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, Types::MONA_FONT, 10,33,129);
    scoreboard_->floats(true);
    accuracy_->floats(true);
    box1_->floats(true);
    box2_->floats(true);
    box3_->floats(true);
    box4_->floats(true);
  }

  DrawBgImage();
}

void SubScreenHandler::PrintCard (const Card& card)
{
  DrawBgImage();
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
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE ||
      screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
  {
    dmaCopy(GetBitmapPtr(), bgGetGfxPtr(bgid_), 256*33);
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

  Graphics::PrintBitmapRegion(x_pos, y_pos, 0, 192, 20, 20, 256, 213,
                              GetBitmapPtr(), RGB15(18,18,28), bgid_,
                              Screen::SUB);
}

void SubScreenHandler::PrintCross (int position)
{
  int x_pos = 0, y_pos = 0;
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
    x_pos = x_position(position), y_pos = 103;
  else if (screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
    x_pos = 10 , y_pos = y_position(position);

  Graphics::PrintBitmapRegion(x_pos, y_pos, 20, 192, 20, 20, 256, 213,
                              GetBitmapPtr(), RGB15(18,18,28), bgid_,
                              Screen::SUB);
}

void SubScreenHandler::PrintScreen (std::string kanji1, std::string kanji2,
                                    std::string kanji3, std::string kanji4,
                                    int score, int answers)
{
  DrawBgImage();
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

void SubScreenHandler::DrawBgImage ()
{
    dmaCopy(GetBitmapPtr(), bgGetGfxPtr(bgid_), 256*256);
    dmaCopy(GetPalPtr(), BG_PALETTE_SUB, 256*2);
}

const unsigned int* SubScreenHandler::GetBitmapPtr ()
{
  if (screen_mode_ == SubScreenMode::CARDS)
    return kanjisubbgBitmap;
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
    return kanjiquizsubbgBitmap;
  if (screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
    return vertical_textboxes_choose_bgBitmap;
  if (screen_mode_ == SubScreenMode::MAIN_MENU)
    return main_menu_bgBitmap;

  return NULL;
}

const unsigned short* SubScreenHandler::GetPalPtr ()
{
  if (screen_mode_ == SubScreenMode::CARDS)
    return kanjisubbgPal;
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
    return kanjiquizsubbgPal;
  if (screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
    return vertical_textboxes_choose_bgPal;
  if (screen_mode_ == SubScreenMode::MAIN_MENU)
    return main_menu_bgPal;

  return NULL;
}

void SubScreenHandler::Fill (unsigned short color)
{
  Graphics::Fill (color, Screen::SUB);
}

void SubScreenHandler::ClearMembers ()
{
  if (screens_handler_)
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
    else if (screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
    {
      screens_handler_->tbh()->DestroyTextBox(scoreboard_);
      screens_handler_->tbh()->DestroyTextBox(accuracy_);
      screens_handler_->tbh()->DestroyTextBox(box1_);
      screens_handler_->tbh()->DestroyTextBox(box2_);
      screens_handler_->tbh()->DestroyTextBox(box3_);
      screens_handler_->tbh()->DestroyTextBox(box4_);
    }
  }
}

SubScreenHandler::~SubScreenHandler ()
{
  ClearMembers();
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

