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
#include <string>
#include "definitions.h"
#include "card.h"
#include "textbox.h"
#include "textboxhandler.h"
#include "graphics.h"
#include "screenshandler.h"
#include "mainscreenhandler.h"

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

  //============================================================================
  if (screen_mode_ == MainScreenMode::KANJI)
  {
    caption_kanji_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, CAPTION_FONT, KM_CAPTION_SIZE,
                  KM_CAPTION_KANJI_X, KM_CAPTION_KANJI_Y);
    kanji_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, NORMAL_FONT, KM_KANJI_SIZE,
                  KM_KANJI_X, KM_KANJI_Y);
    kanji_->floats(true);
    caption_on_reading_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, CAPTION_FONT, KM_CAPTION_SIZE,
                  KM_CAPTION_ON_READING_X, KM_CAPTION_ON_READING_Y);
    on_reading_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, NORMAL_FONT, KM_NORMAL_SIZE,
                  KM_ON_READING_X, KM_ON_READING_Y);
    caption_kun_reading_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, CAPTION_FONT, KM_CAPTION_SIZE,
                  KM_CAPTION_KUN_READING_X, KM_CAPTION_KUN_READING_Y);
    kun_reading_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, NORMAL_FONT, KM_NORMAL_SIZE,
                  KM_KUN_READING_X, KM_KUN_READING_Y);
    caption_translation_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, CAPTION_FONT, KM_CAPTION_SIZE,
                  KM_CAPTION_TRANSLATION_X, KM_CAPTION_TRANSLATION_Y);
    translation_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, NORMAL_FONT, KM_NORMAL_SIZE,
                  KM_TRANSLATION_X, KM_TRANSLATION_Y);
    caption_example_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, CAPTION_FONT, KM_CAPTION_SIZE,
                  KM_CAPTION_EXAMPLE_X, KM_CAPTION_EXAMPLE_Y);
    example_kanji_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, NORMAL_FONT, KM_NORMAL_SIZE,
                  KM_EXAMPLE_KANJI_X, KM_EXAMPLE_KANJI_Y);
    example_reading_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, NORMAL_FONT, KM_NORMAL_SIZE,
                  KM_EXAMPLE_READING_X, KM_EXAMPLE_READING_Y);
    example_translation_ = screens_handler_->tbh()->
      NewTextBox (Screen::MAIN, bgid_, NORMAL_FONT, KM_NORMAL_SIZE,
                  KM_EXAMPLE_TRANSLATION_X, KM_EXAMPLE_TRANSLATION_X);

    // Text that won't change
    caption_kanji_->text("Kanji");
    caption_on_reading_->text("on reading");
    caption_kun_reading_->text("kun reading");
    caption_translation_->text("Translation");
    caption_example_->text("Example");

    // Make all but the kanji and its caption invisible.
    caption_on_reading_->visible(false);
    caption_kun_reading_->visible(false);
    caption_translation_->visible(false);
    caption_example_->visible(false);
    on_reading_->visible(false);
    kun_reading_->visible(false);
    translation_->visible(false);
    example_kanji_->visible(false);
    example_reading_->visible(false);
    example_translation_->visible(false);
  }
  //============================================================================
  else if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES ||
           screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES_VISIBLE)
  {
    if (boxes_number_ >= 1)
    {
      caption_box1_ = screens_handler_->tbh()->
        NewTextBox (Screen::MAIN, bgid_, CAPTION_FONT, VTB_CAPTION_SIZE,
                    VTB_CAPTION_BOX1_X, VTB_CAPTION_BOX1_Y);
      box1_ = screens_handler_->tbh()->
        NewTextBox (Screen::MAIN, bgid_, NORMAL_FONT, VTB_NORMAL_SIZE,
                    VTB_BOX1_X, VTB_BOX1_Y);
      if (boxes_number_ >= 2)
      {
        caption_box2_ = screens_handler_->tbh()->
          NewTextBox (Screen::MAIN, bgid_, CAPTION_FONT,
                      VTB_CAPTION_SIZE, VTB_CAPTION_BOX2_X,
                      VTB_CAPTION_BOX2_Y);
        box2_ = screens_handler_->tbh()->
          NewTextBox (Screen::MAIN, bgid_, NORMAL_FONT,
                      VTB_NORMAL_SIZE, VTB_BOX2_X, VTB_BOX2_Y);
        if (boxes_number_ >= 3)
        {
          caption_box3_ = screens_handler_->tbh()->
            NewTextBox (Screen::MAIN, bgid_, CAPTION_FONT,
                        VTB_CAPTION_SIZE, VTB_CAPTION_BOX3_X,
                        VTB_CAPTION_BOX3_Y);
          box3_ = screens_handler_->tbh()->
            NewTextBox (Screen::MAIN, bgid_, NORMAL_FONT,
                        VTB_NORMAL_SIZE, VTB_BOX3_X, VTB_BOX3_Y);
        }
      }
    }
  }

  if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES)
  {
    if (boxes_number_ >= 2)
    {
      caption_box2_->visible(false);
      box2_->visible(false);
      if (boxes_number_>= 3)
      {
        caption_box3_->visible(false);
        box3_->visible(false);
      }
    }
  }

  DrawBgImage ();
  Graphics::SetColors();
}

void MainScreenHandler::PrintCard (const Card& card, bool convert_to_romaji)
{
  DrawBgImage ();
  ResetTextBoxesPositions ();
 	if (screen_mode_ == MainScreenMode::KANJI)
  {
    kanji_->text(card.symbol());
    on_reading_->text(card.reading(), convert_to_romaji);
    kun_reading_->text(card.reading2(), convert_to_romaji);
    translation_->text(card.translation());
    example_kanji_->text(card.example_symbol());
    example_reading_->text(card.example_reading(), convert_to_romaji);
    example_translation_->text(card.example_translation());

    // Make all but the kanji and its caption invisible.
    caption_on_reading_->visible(false);
    caption_kun_reading_->visible(false);
    caption_translation_->visible(false);
    caption_example_->visible(false);
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
    bool convert_to_romaji1, convert_to_romaji2, convert_to_romaji3;
    convert_to_romaji1 = convert_to_romaji2 = convert_to_romaji3 = false;
    std::string box1text, box2text, box3text;
    if (game_mode_ == GameMode::KANJI_QUIZ)
    {
      box1text = card.translation();
      box2text = card.reading();
      box3text = card.reading2();
      convert_to_romaji2 = convert_to_romaji3 = true;
    }
    else if (game_mode_ == GameMode::VOCABULARY)
    {
      box1text = card.symbol();
      box2text = card.reading();
      box3text = card.translation();
      convert_to_romaji2 = true;
    }
    else if (game_mode_ == GameMode::VOCABULARY_QUIZ)
    {
      box1text = card.translation();
      box2text = card.reading();
      convert_to_romaji2 = true;
    }

    if (boxes_number_ >= 1)
    {
      box1_->text(box1text, convert_to_romaji1);
      if (boxes_number_ >= 2)
      {
        box2_->text(box2text, convert_to_romaji2);
        if (boxes_number_ >= 3)
          box3_->text(box3text, convert_to_romaji3);
      }
    }

    if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES)
    {
      if (boxes_number_ >= 2)
      {
        caption_box2_->visible(false);
        box2_->visible(false);
        if (boxes_number_>= 3)
        {
          caption_box3_->visible(false);
          box3_->visible(false);
        }
      }
    }

    screens_handler_->tbh()->PrintAll(Screen::MAIN);
  }
}

void MainScreenHandler::Scroll (int sx, int sy)
{
  if (scroll_y_ != sy)
  {
    bgSetScroll(bgid_,sx,sy);
    bgUpdate();
    scroll_y_ = sy;
  }
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
  if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES)
  {
    if (boxes_number_ >= 2 && box2_->visible())
      if (boxes_number_ >= 3 && !box3_->visible())
      {
        caption_box3_->visible(true);
        caption_box3_->Print();
        box3_->visible(true);
        box3_->Print();
      }
      else
          all_visible = true;
    else if (boxes_number_ >= 2)
    {
      caption_box2_->visible(true);
      caption_box2_->Print();
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
            caption_example_->visible(true);
            example_kanji_->visible(true);
            example_reading_->visible(true);
            example_translation_->visible(true);
            screens_handler_->tbh()->PrintTextBox(caption_example_);
            screens_handler_->tbh()->PrintTextBox(example_kanji_);
            screens_handler_->tbh()->PrintTextBox(example_reading_);
            screens_handler_->tbh()->PrintTextBox(example_translation_);
          }
        }
        else
        {
          caption_translation_->visible(true);
          screens_handler_->tbh()->PrintTextBox(caption_translation_);
          translation_->visible(true);
          screens_handler_->tbh()->PrintTextBox(translation_);
        }
      }
      else
      {
        caption_kun_reading_->visible(true);
        screens_handler_->tbh()->PrintTextBox(caption_kun_reading_);
        kun_reading_->visible(true);
        screens_handler_->tbh()->PrintTextBox(kun_reading_);
      }
    }
    else
    {
      caption_on_reading_->visible(true);
      screens_handler_->tbh()->PrintTextBox(caption_on_reading_);
      on_reading_->visible(true);
      screens_handler_->tbh()->PrintTextBox(on_reading_);
    }
  }
  return all_visible;
}


void MainScreenHandler::DrawBgImage ()
{
  Scroll(0,0);
  scroll_y_ = 0;
  if (screen_mode_ == MainScreenMode::SPLASH_SCREEN)
    Graphics::SplashImage(Graphics::GetBitmapPtr(screen_mode_),
                          Graphics::GetPalPtr(screen_mode_),
                          Screen::MAIN, bgid_, Graphics::SplashEffect::APPEAR);
  else
  {
    dmaCopy(Graphics::GetBitmapPtr(screen_mode_), bgGetGfxPtr(bgid_), 256*256);
    dmaCopy(Graphics::GetPalPtr(screen_mode_), BG_PALETTE, 256*2);
  }
}

void MainScreenHandler::Fill (unsigned short color)
{
  Graphics::Fill (color, Screen::MAIN);
}

void MainScreenHandler::ResetTextBoxesPositions ()
{
  if (screens_handler_)
  {
    if (screen_mode_ == MainScreenMode::KANJI)
    {
      caption_kanji_->SetProperties(KM_CAPTION_KANJI_X,
                                    KM_CAPTION_KANJI_Y, 0, 0);
      kanji_->SetProperties(KM_KANJI_X, KM_KANJI_Y, 0, 0);
      caption_on_reading_->SetProperties(KM_CAPTION_ON_READING_X,
                                         KM_CAPTION_ON_READING_Y, 0, 0);
      on_reading_->SetProperties(KM_ON_READING_X, KM_ON_READING_Y, 0, 0);
      caption_kun_reading_->SetProperties(KM_CAPTION_KUN_READING_X,
                                          KM_CAPTION_KUN_READING_Y, 0, 0);
      kun_reading_->SetProperties(KM_KUN_READING_X, KM_KUN_READING_Y, 0, 0);
      caption_translation_->SetProperties(KM_CAPTION_TRANSLATION_X,
                                          KM_CAPTION_TRANSLATION_Y, 0, 0);
      translation_->SetProperties(KM_TRANSLATION_X, KM_TRANSLATION_Y, 0, 0);
      caption_example_->SetProperties(KM_CAPTION_EXAMPLE_X,
                                      KM_CAPTION_EXAMPLE_Y, 0, 0);
      example_kanji_->SetProperties(KM_EXAMPLE_KANJI_X,
                                    KM_EXAMPLE_KANJI_Y, 0, 0);
      example_reading_->SetProperties(KM_EXAMPLE_READING_X,
                                      KM_EXAMPLE_READING_Y, 0, 0);
      example_translation_->SetProperties(KM_EXAMPLE_TRANSLATION_X,
                                          KM_EXAMPLE_TRANSLATION_Y, 0, 0);
    }
    else if (screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES ||
             screen_mode_ == MainScreenMode::VERTICAL_TEXTBOXES_VISIBLE)
    {
      if (boxes_number_ >= 1)
      {
        caption_box1_->SetProperties(VTB_CAPTION_BOX1_X,
                                     VTB_CAPTION_BOX1_Y, 0, 0);
        box1_->SetProperties(VTB_BOX1_X, VTB_BOX1_Y, 0, 0);
        if (boxes_number_ >= 2)
        {
          caption_box2_->SetProperties(VTB_CAPTION_BOX2_X,
                                       VTB_CAPTION_BOX2_Y, 0, 0);
          box2_->SetProperties(VTB_BOX2_X, VTB_BOX2_Y, 0, 0);
          if (boxes_number_ >= 3)
          {
            caption_box3_->SetProperties(VTB_CAPTION_BOX3_X,
                                         VTB_CAPTION_BOX3_Y, 0, 0);
            box3_->SetProperties(VTB_BOX3_X, VTB_BOX3_Y, 0, 0);
          }
        }
      }
    }
  }
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
