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
#include "definitions.h"
#include "card.h"
#include "textbox.h"
#include "textboxhandler.h"
#include "screenshandler.h"
#include "subscreenhandler.h"
#include "graphics.h"

using namespace Types;

/// Returns the position for the tick or cross on the kanji number 'position'
inline int x_position (int position)
{
  int x_pos = 0;
  switch (position)
  {
    case 1: x_pos = TICK_CROSS_X_1;
    break;
    case 2: x_pos = TICK_CROSS_X_2;
    break;
    case 3: x_pos = TICK_CROSS_X_3;
    break;
    case 4: x_pos = TICK_CROSS_X_4;
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
    case 1: y_pos = TICK_CROSS_Y_1;
    break;
    case 2: y_pos = TICK_CROSS_Y_2;
    break;
    case 3: y_pos = TICK_CROSS_Y_3;
    break;
    case 4: y_pos = TICK_CROSS_Y_4;
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
                             GameMode::mode game_mode,
                             ScreensHandler* screens_handler)
{
  SetMode (screen_mode, game_mode, screens_handler);
}

/// \param screen_mode Mode of the screen
void SubScreenHandler::SwitchMode (SubScreenMode::mode screen_mode,
                                   GameMode::mode game_mode)
{
  ClearMembers ();
  SetMode (screen_mode, game_mode, screens_handler_);
}

/**
 *  \param screen_mode Mode of the screen
 *  \param screens_handler Already created and initialized screens handler
 */
void SubScreenHandler::SetMode (SubScreenMode::mode screen_mode,
                                GameMode::mode game_mode,
                                ScreensHandler* screens_handler)
{
  screen_mode_ = screen_mode;
  game_mode_ = game_mode;
  screens_handler_ = screens_handler;

  if (screen_mode_ == SubScreenMode::CARDS)
  {
    card_number_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, CAPTION_FONT, CARDS_TEXT_SIZE,
         CARDS_TEXT_X, CARDS_TEXT_Y);
    card_number_->floats(true);
  }
  else if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
  {
    scoreboard_  =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, CAPTION_FONT, KC_CAPTION_SIZE,
         KC_SCOREBOARD_X, KC_SCOREBOARD_Y);
    accuracy_  =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, CAPTION_FONT, KC_CAPTION_SIZE,
         KC_ACCURACY_X, KC_ACCURACY_Y);
    box1_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, NORMAL_FONT, KC_NORMAL_SIZE,
         KC_BOX1_X, KC_BOX1_Y);
    box2_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, NORMAL_FONT, KC_NORMAL_SIZE,
         KC_BOX2_X, KC_BOX2_Y);
    box3_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, NORMAL_FONT, KC_NORMAL_SIZE,
         KC_BOX3_X, KC_BOX3_Y);
    box4_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, NORMAL_FONT, KC_NORMAL_SIZE,
         KC_BOX4_X, KC_BOX4_Y);
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
        (Screen::SUB, bgid_, CAPTION_FONT, VTBC_CAPTION_SIZE,
         VTBC_SCOREBOARD_X, VTBC_SCOREBOARD_Y);
    accuracy_  =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, CAPTION_FONT, VTBC_CAPTION_SIZE,
         VTBC_ACCURACY_X, VTBC_ACCURACY_Y);
    box1_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, NORMAL_FONT, VTBC_NORMAL_SIZE,
         VTBC_BOX1_X, VTBC_BOX1_Y);
    box2_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, NORMAL_FONT, VTBC_NORMAL_SIZE,
         VTBC_BOX2_X, VTBC_BOX2_Y);
    box3_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, NORMAL_FONT, VTBC_NORMAL_SIZE,
         VTBC_BOX3_X, VTBC_BOX3_Y);
    box4_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, NORMAL_FONT, VTBC_NORMAL_SIZE,
         VTBC_BOX4_X, VTBC_BOX4_Y);
    scoreboard_->floats(true);
    accuracy_->floats(true);
    box1_->floats(true);
    box2_->floats(true);
    box3_->floats(true);
    box4_->floats(true);
  }
  else if (screen_mode_ == SubScreenMode::OPTIONS_GRADE_STROKES)
  {
    mode_title_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, CAPTION_FONT, 14,
         48, 12);
    caption_grade_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, CAPTION_FONT, 10,
         30, 67);
    caption_strokes_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, CAPTION_FONT, 10,
         25, 97);
    grade_min_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, CAPTION_FONT, 10,
         113, 68);
    grade_max_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, CAPTION_FONT, 10,
         190, 68);
    strokes_min_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, CAPTION_FONT, 10,
         107, 97);
    strokes_max_ =
      screens_handler_->tbh()->NewTextBox
        (Screen::SUB, bgid_, CAPTION_FONT, 10,
         185, 97);

    mode_title_->floats(true);
    caption_grade_->floats(true);
    caption_strokes_->floats(true);
    grade_min_->floats(true);
    grade_max_->floats(true);
    strokes_min_->floats(true);
    strokes_max_->floats(true);

    mode_title_->text("Options");
    caption_grade_->text("Grade");
    caption_strokes_->text("Strokes");
  }

  DrawBgImage();
  Graphics::SetColors();

  if (screen_mode_ == SubScreenMode::OPTIONS_GRADE_STROKES)
  {
    mode_title_->Print();
    caption_grade_->Print();
    caption_strokes_->Print();
  }
}

void SubScreenHandler::PrintCard (const Card& card)
{
  DrawBgImage();
 	if (screen_mode_ == SubScreenMode::CARDS)
  {
    char* card_number_text = new char [MAX_CARD_NUMBER_TEXT_LENGTH];
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
    dmaCopy(Graphics::GetSubBitmapPtr(screen_mode_, game_mode_),
            bgGetGfxPtr(bgid_), 256*33);
    char* score_text = new char [MAX_SCORE_TEXT_LENGTH];
    sprintf(score_text, "Score: %i",score);
    scoreboard_->text(score_text);
    delete score_text;

    scoreboard_->Print();

    char* accuracy_text = new char [MAX_ACCURACY_TEXT_LENGTH];
    sprintf(accuracy_text, "Accuracy: %i%%",score*100/answers);
    accuracy_->text(accuracy_text);
    delete accuracy_text;

    accuracy_->Print();
  }
}

void SubScreenHandler::PrintOptions(unsigned grade_min, unsigned grade_max,
                                    unsigned strokes_min, unsigned strokes_max)
{
  if (screen_mode_ == SubScreenMode::OPTIONS_GRADE_STROKES)
  {
    char s [3];
    sprintf(s, "%i", grade_min);
    grade_min_->text(s);
    sprintf(s, "%i", grade_max);
    grade_max_->text(s);
    sprintf(s, "%2i", strokes_min);
    strokes_min_->text(s);
    sprintf(s, "%2i", strokes_max);
    strokes_max_->text(s);

    Graphics::PrintBitmapRegion(106,68, 106,68, 102, 50, SCREEN_WIDTH, SCREEN_HEIGHT, Graphics::GetSubBitmapPtr(screen_mode_, game_mode_), RGB15(18,18,28), bgid_, Screen::SUB);
    grade_min_->Print();
    grade_max_->Print();
    strokes_min_->Print();
    strokes_max_->Print();
  }
}

void SubScreenHandler::PrintTick (int position)
{
  int x_pos = 0, y_pos = 0;
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
    x_pos = x_position(position), y_pos = KC_TICK_CROSS_Y;
  else if (screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
    x_pos = KC_TICK_CROSS_X , y_pos = y_position(position);

  Graphics::PrintBitmapRegion(x_pos, y_pos, 0, 192, 20, 20, 256, 213,
                              Graphics::GetSubBitmapPtr(screen_mode_, game_mode_),
                              RGB15(18,18,28), bgid_, Screen::SUB);
}

void SubScreenHandler::PrintCross (int position)
{
  int x_pos = 0, y_pos = 0;
  if (screen_mode_ == SubScreenMode::KANJI_CHOOSE)
    x_pos = x_position(position), y_pos = KC_TICK_CROSS_Y;
  else if (screen_mode_ == SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE)
    x_pos = KC_TICK_CROSS_X , y_pos = y_position(position);

  Graphics::PrintBitmapRegion(x_pos, y_pos, 20, 192, 20, 20, 256, 213,
                              Graphics::GetSubBitmapPtr(screen_mode_, game_mode_),
                              RGB15(18,18,28), bgid_, Screen::SUB);
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

    char* score_text = new char [MAX_SCORE_TEXT_LENGTH];
    sprintf(score_text, "Score: %i",score);
    scoreboard_->text(score_text);
    delete score_text;

    scoreboard_->Print();

    char* accuracy_text = new char [MAX_ACCURACY_TEXT_LENGTH];
    sprintf(accuracy_text, "Accuracy: %i%%",score*100/answers);
    accuracy_->text(accuracy_text);
    delete accuracy_text;

    screens_handler_->tbh()->PrintAll(Screen::SUB);
  }
}

void SubScreenHandler::DrawBgImage ()
{
    dmaCopy(Graphics::GetSubBitmapPtr(screen_mode_, game_mode_),
            bgGetGfxPtr(bgid_), 256*256);
    dmaCopy(Graphics::GetSubPalPtr(screen_mode_, game_mode_), BG_PALETTE_SUB, 256*2);
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
    else if (screen_mode_ == SubScreenMode::OPTIONS_GRADE_STROKES)
    {
      screens_handler_->tbh()->DestroyTextBox(mode_title_);
      screens_handler_->tbh()->DestroyTextBox(caption_grade_);
      screens_handler_->tbh()->DestroyTextBox(caption_strokes_);
      screens_handler_->tbh()->DestroyTextBox(grade_min_);
      screens_handler_->tbh()->DestroyTextBox(grade_max_);
      screens_handler_->tbh()->DestroyTextBox(strokes_min_);
      screens_handler_->tbh()->DestroyTextBox(strokes_max_);
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

