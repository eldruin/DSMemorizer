/// \file mainscreenhandler.cpp Handler for the main screen
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
#include "graphics.h"
#include "screenshandler.h"
#include "mainscreenhandler.h"

#include "kanjibg.h"
#include "goibg.h"
#include "splash_main_bg.h"

using namespace Types;

MainScreenHandler::MainScreenHandler()
{
  videoSetMode (MODE_5_2D);
  vramSetBankA (VRAM_A_MAIN_BG_0x06000000);

  bgid_ = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
  // To check later
  screens_handler_ = NULL;
}

/**
 *  \param screen_mode Mode of the screen
 *  \param game_mode Game mode
 *  \param screens_handler Already created and initialized screens handler
 *  \param boxes_number Number of text boxes to be used [0-3].
 */
void MainScreenHandler::Init (MainScreenMode::mode screen_mode,
                              GameMode::mode game_mode,
                              ScreensHandler* screens_handler,
                              int boxes_number)
{
  SetMode(screen_mode, game_mode, screens_handler, boxes_number);
}

/**
 *  \param screen_mode Mode of the screen
 *  \param game_mode Game mode
 *  \param boxes_number Number of text boxes to be used [0-3].
 */
void MainScreenHandler::SwitchMode (MainScreenMode::mode screen_mode,
                                    GameMode::mode game_mode,
                                    int boxes_number)
{
  ClearMembers ();
  SetMode(screen_mode, game_mode, screens_handler_, boxes_number);
}

/**
 *  \param screen_mode Mode of the screen
 *  \param game_mode Game mode
 *  \param screens_handler Already created and initialized screens handler
 *  \param boxes_number Number of text boxes to be used [0-3].
 */
void MainScreenHandler::SetMode (MainScreenMode::mode screen_mode,
                                 GameMode::mode game_mode,
                                 ScreensHandler* screens_handler,
                                 int boxes_number)
{
  screens_handler_ = screens_handler;
  game_mode_ = game_mode;
  screen_mode_ = screen_mode;
  boxes_number_ = boxes_number;

  short white_color;
  //============================================================================
  if (screen_mode_ == MainScreenMode::KANJI)
  {
    caption_kanji_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, VERA_FONT, 8,15,17,60,0);
    kanji_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, MONA_FONT, 30,12,60,100,0);
    kanji_->floats(true);
    caption_on_reading_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, VERA_FONT, 8,70,17,100,0);
    on_reading_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, MONA_FONT, 10,70,30,100,0);
    caption_kun_reading_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, VERA_FONT, 8,70,35,100,0);
    kun_reading_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, MONA_FONT, 10,70,40,100,0);
    caption_translation_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, VERA_FONT, 8,10,100,100,0);
    translation_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, MONA_FONT, 10,10,100,235,0);
    caption_example_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, VERA_FONT, 8,10,130,100,0);
    example_kanji_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, MONA_FONT, 10,10,135,235,0);
    example_reading_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, MONA_FONT, 10,10,160,235,0);
    example_translation_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, MONA_FONT, 10,10,190,235,0);

    // Text that won't change
    caption_kanji_->text("Kanji");
    caption_on_reading_->text("on reading");
    caption_kun_reading_->text("kun reading");
    caption_translation_->text("Translation");
    caption_example_->text("Example");

    // Make all but the kanji and captions invisible.
    on_reading_->visible(false);
    kun_reading_->visible(false);
    translation_->visible(false);
    example_kanji_->visible(false);
    example_reading_->visible(false);
    example_translation_->visible(false);

    white_color = RGB15(18,18,28);
  }
  //============================================================================
  else if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES ||
           screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES_VISIBLE)
  {
    if (boxes_number_ >= 1)
    {
      caption_box1_ = screens_handler_->tbh()->
        NewTextBox (Screen::MAIN, bgid_, Types::VERA_FONT, 8,20,30,225,0);
      box1_ = screens_handler_->tbh()->
        NewTextBox (Screen::MAIN, bgid_, Types::MONA_FONT, 10,20,40,225,0);
      if (boxes_number_ >= 2)
      {
        caption_box2_ = screens_handler_->tbh()->
          NewTextBox (Screen::MAIN, bgid_, Types::VERA_FONT, 8,20,50,225,0);
        box2_ = screens_handler_->tbh()->
          NewTextBox (Screen::MAIN, bgid_, Types::MONA_FONT, 10,20,60,225,0);
        if (boxes_number_ >= 3)
        {
          caption_box3_ = screens_handler_->tbh()->
            NewTextBox (Screen::MAIN, bgid_, Types::VERA_FONT, 8,20,70,225,0);
          box3_ = screens_handler_->tbh()->
            NewTextBox (Screen::MAIN, bgid_, Types::MONA_FONT,
              10,20,80,225,0);
        }
      }
    }

    white_color = RGB15(18,18,28);
  }

  if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES)
  {
    if (boxes_number_ >= 2)
    {
      box2_->visible(false);
      if (boxes_number_>= 3)
        box3_->visible(false);
    }
  }
  if (screen_mode_ != MainScreenMode::SPLASH_SCREEN)
  {
    BG_PALETTE[Color::BLACK] = RGB15(0,0,0);
    BG_PALETTE[Color::GREY] = RGB15(15,15,15);
    BG_PALETTE[Color::WHITE] = white_color;
  }

  DrawBgImage ();
}

void MainScreenHandler::PrintCard (const Card& card)
{
  DrawBgImage ();
 	if (screen_mode_ == MainScreenMode::KANJI)
  {
    kanji_->text(card.symbol());
    on_reading_->text(card.reading());
    kun_reading_->text(card.reading2());
    translation_->text(card.translation());
    example_kanji_->text(card.example_symbol());
    example_reading_->text(card.example_reading());
    example_translation_->text(card.example_translation());

    // Make all but the kanji and captions invisible.
    on_reading_->visible(false);
    kun_reading_->visible(false);
    translation_->visible(false);
    example_kanji_->visible(false);
    example_reading_->visible(false);
    example_translation_->visible(false);

    screens_handler_->tbh()->PrintAll(Screen::MAIN);
  }
  else if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES ||
           screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES_VISIBLE)
  {
    if (boxes_number_ >= 1)
    {
      box1_->text(card.translation());
      if (boxes_number_ >= 2)
      {
        box2_->text(card.reading());
        if (boxes_number_ >= 3)
          box3_->text(card.reading2());
        if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES)
        {
          if (boxes_number_ >= 2)
          {
            box2_->visible(false);
            if (boxes_number_>= 3)
              box3_->visible(false);
          }
        }
      }
    }
    screens_handler_->tbh()->PrintAll(Screen::MAIN);
  }
}

void MainScreenHandler::Scroll (int sx, int sy) const
{
  bgSetScroll(bgid_,0,sy);
  bgUpdate();
}

void MainScreenHandler::Captions (std::string box1, std::string box2,
                                  std::string box3)
{
  if (boxes_number_ >= 1)
  {
    caption_box1_->text(box1);
    if (boxes_number_ >= 2)
    {
      caption_box2_->text(box2);
      if (boxes_number_ >= 3)
        caption_box3_->text(box3);
    }
  }
}

bool MainScreenHandler::ViewNext ()
{
  bool all_visible = false;
  if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES ||
      screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES_VISIBLE)
  {
    if (boxes_number_ >= 2 && box2_->visible())
      if (boxes_number_ >= 3 && !box3_->visible())
      {
        box3_->visible(true);
        box3_->Print();
      }
      else
          all_visible = true;
    else if (boxes_number_ >= 2)
    {
      box2_->visible(true);
      box2_->Print();
    }
    else
      all_visible = true;
  }
  else if (screen_mode_ == MainScreenMode::KANJI)
  {
    if (on_reading_->visible())
    {
      if (kun_reading_->visible())
      {
        if (translation_->visible())
        {
          if (example_kanji_->visible())
            all_visible = true;
          else
          {
            example_kanji_->visible(true);
            example_reading_->visible(true);
            example_translation_->visible(true);
            example_kanji_->Print();
            example_reading_->Print();
            example_translation_->Print();
          }
        }
        else
        {
          translation_->visible(true);
          translation_->Print();
        }
      }
      else
      {
        kun_reading_->visible(true);
        kun_reading_->Print();
      }
    }
    else
    {
      on_reading_->visible(true);
      on_reading_->Print();
    }
  }
  return all_visible;
}


void MainScreenHandler::DrawBgImage ()
{
  if (screen_mode_ == MainScreenMode::KANJI)
  {
    dmaCopy(kanjibgBitmap, bgGetGfxPtr(bgid_), 256*256);
    dmaCopy(kanjibgPal, BG_PALETTE, 256*2);
  }
  else if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES ||
           screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES_VISIBLE)
  {
    if (game_mode_ == GameMode::KANJI_QUIZ)
    {
      dmaCopy(kanjibgBitmap, bgGetGfxPtr(bgid_), 256*256);
      dmaCopy(kanjibgPal, BG_PALETTE, 256*2);
    }
    else if (game_mode_ == GameMode::VOCABULARY ||
             game_mode_ == GameMode::VOCABULARY_QUIZ)
    {
      dmaCopy(goibgBitmap, bgGetGfxPtr(bgid_), 256*256);
      dmaCopy(goibgPal, BG_PALETTE, 256*2);
    }
  }
  else if (screen_mode_ == MainScreenMode::SPLASH_SCREEN)
    Graphics::SplashImage(splash_main_bgBitmap, splash_main_bgPal,
                          Screen::MAIN, bgid_, Graphics::SplashEffect::APPEAR);
}

void MainScreenHandler::ClearMembers ()
{
  if (screens_handler_)
  {
    if (screen_mode_ == MainScreenMode::KANJI)
    {
      screens_handler_->tbh()->DestroyTextBox(caption_kanji_);
      screens_handler_->tbh()->DestroyTextBox(kanji_);
      screens_handler_->tbh()->DestroyTextBox(caption_on_reading_);
      screens_handler_->tbh()->DestroyTextBox(on_reading_);
      screens_handler_->tbh()->DestroyTextBox(caption_kun_reading_);
      screens_handler_->tbh()->DestroyTextBox(kun_reading_);
      screens_handler_->tbh()->DestroyTextBox(caption_translation_);
      screens_handler_->tbh()->DestroyTextBox(translation_);
      screens_handler_->tbh()->DestroyTextBox(caption_example_);
      screens_handler_->tbh()->DestroyTextBox(example_kanji_);
      screens_handler_->tbh()->DestroyTextBox(example_reading_);
      screens_handler_->tbh()->DestroyTextBox(example_translation_);
    }
    else if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES ||
             screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES_VISIBLE)
    {
      if (boxes_number_ >= 1)
      {
        screens_handler_->tbh()->DestroyTextBox(box1_);
        screens_handler_->tbh()->DestroyTextBox(caption_box1_);
        if (boxes_number_ >= 2)
        {
          screens_handler_->tbh()->DestroyTextBox(box2_);
          screens_handler_->tbh()->DestroyTextBox(caption_box2_);
          if (boxes_number_ >= 3)
          {
            screens_handler_->tbh()->DestroyTextBox(box3_);
            screens_handler_->tbh()->DestroyTextBox(caption_box3_);
          }
        }
      }
    }
  }
}

MainScreenHandler::~MainScreenHandler()
{
  ClearMembers();
}
