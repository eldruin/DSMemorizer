/// \file dsmemorizer.cpp Game main loop and main menu.
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
#include <stdio.h>
#include <malloc.h>
#include <fat.h>
#include <unistd.h>		// filesystem functions
#include <maxmod9.h>
#include <string>
#include <vector>

#include "definitions.h"
#include "types.h"
#include "card.h"
#include "xmlparser.h"
#include "graphics.h"
#include "soundhandler.h"
#include "screenshandler.h"
#include "subscreenhandler.h"
#include "mainscreenhandler.h"
#include "dsmemorizer.h"

using namespace Types;

void DSMemorizer::Init ()
{
  romaji_conversion = false;
  sound_handler_.Init();
  sound_handler_.PlayEffect(SoundHandler::THEME);
  screens_handler_.Init();
  main_screen_handler_.Init (MainScreenMode::SPLASH_SCREEN,
                             GameMode::SPLASH_SCREEN, &screens_handler_);
  sub_screen_handler_.Init (SubScreenMode::MAIN_MENU, GameMode::MAIN_MENU,
                            &screens_handler_);

  // Vertical scroll
  int sy = 0;

  int height = 256;
  int keys = 0;
  touchPosition touch;

  // Loop
  while(true)
  {
      int selected_button = 0;
      scanKeys();

      touchRead(&touch);

      keys = keysHeld();

      if (touch.px > TP_MAIN_MENU_BUTTON1_X1 &&
          touch.px < TP_MAIN_MENU_BUTTON1_X2 &&
          touch.py > TP_MAIN_MENU_BUTTON1_Y1 &&
          touch.py < TP_MAIN_MENU_BUTTON1_Y2)
        selected_button = 1;
      if (touch.px > TP_MAIN_MENU_BUTTON2_X1 &&
          touch.px < TP_MAIN_MENU_BUTTON2_X2 &&
          touch.py > TP_MAIN_MENU_BUTTON2_Y1 &&
          touch.py < TP_MAIN_MENU_BUTTON2_Y2)
        selected_button = 2;
      if (touch.px > TP_MAIN_MENU_BUTTON3_X1 &&
          touch.px < TP_MAIN_MENU_BUTTON3_X2 &&
          touch.py > TP_MAIN_MENU_BUTTON3_Y1 &&
          touch.py < TP_MAIN_MENU_BUTTON3_Y2)
        selected_button = 3;
      if (touch.px > TP_MAIN_MENU_BUTTON4_X1 &&
          touch.px < TP_MAIN_MENU_BUTTON4_X2 &&
          touch.py > TP_MAIN_MENU_BUTTON4_Y1 &&
          touch.py < TP_MAIN_MENU_BUTTON4_Y2)
        selected_button = 4;

      if (sy < 0) sy = 0;
      if (sy >= height - 192) sy = height - 1 - 192;

      if (selected_button)
      {
        sound_handler_.PlayEffect(SoundHandler::ACTION);
        sound_handler_.StopEffect(SoundHandler::THEME);
        sound_handler_.UnloadEffect(SoundHandler::THEME);
        switch(selected_button)
        {
          case 1: KanjiMode ();
          break;
          case 2: KanjiQuizMode ();
          break;
          case 3: VocabularyMode ();
          break;
          case 4: VocabularyQuizMode ();
          break;
        }
        sound_handler_.PlayEffect(SoundHandler::THEME);
        sub_screen_handler_.Fill(0);
        main_screen_handler_.SwitchMode (MainScreenMode::SPLASH_SCREEN,
                                         GameMode::MAIN_MENU);
        sub_screen_handler_.SwitchMode (SubScreenMode::MAIN_MENU,
                                        GameMode::MAIN_MENU);
      }
      swiWaitForVBlank();
   }

}

void DSMemorizer::KanjiMode ()
{
  xmlparser_.Init("/db/kanjis.xml");

  main_screen_handler_.SwitchMode (MainScreenMode::KANJI, GameMode::KANJI);
  sub_screen_handler_.SwitchMode (SubScreenMode::CARDS, GameMode::KANJI);

  int sy = 0;
  // Shown card
  int card = 1;
  // Previously shown card
  int previous_card = 0;

  int height = 256;
  int keys = 0;
  touchPosition touch;
  unsigned grade_min, grade_max, strokes_min, strokes_max;
  // By default: all the kanjis
  grade_min = MIN_GRADE, grade_max = MAX_GRADE;
  strokes_min = MIN_STROKES, strokes_max = MAX_STROKES;

  // Loop
  bool done = false;
  while(!(keys & KEY_B) && !done)
  {
      scanKeys();

      touchRead(&touch);

      keys = keysDown();

      if ((keys & KEY_LEFT) || (keys & KEY_RIGHT) || (keys & KEY_A) ||
          (keys & KEY_X) || (keys & KEY_Y))
        sound_handler_.PlayEffect(SoundHandler::ACTION);

      if (keysHeld() & KEY_UP) sy--;
      if (keysHeld() & KEY_DOWN) sy++;
      if (keys & KEY_LEFT) card--;
      if (keys & KEY_RIGHT) card++;
      if (keys & KEY_A)
        if (main_screen_handler_.ViewNext())
          ++card;
      if (keys & KEY_X)
        card = rand()%xmlparser_.QueryResultSize(grade_min, grade_max,
                                                 strokes_min, strokes_max) + 1;
      if (keys & KEY_Y)
      {
        SetOptions(grade_min, grade_max, strokes_min, strokes_max);
        card = 1;
        previous_card = 0;
        sub_screen_handler_.SwitchMode (SubScreenMode::CARDS, GameMode::KANJI);
      }

      if (touch.px > TP_LEFT_BOTTOM_IMAGE_X1 &&
          touch.px < TP_LEFT_BOTTOM_IMAGE_X2 &&
          touch.py > TP_LEFT_BOTTOM_IMAGE_Y1 &&
          touch.py < TP_LEFT_BOTTOM_IMAGE_Y2)
        done = true;

      if (touch.px > TP_RIGHT_BOTTOM_IMAGE_X1 &&
          touch.px < TP_RIGHT_BOTTOM_IMAGE_X2 &&
          touch.py > TP_RIGHT_BOTTOM_IMAGE_Y1 &&
          touch.py < TP_RIGHT_BOTTOM_IMAGE_Y2)
      {
        SetOptions(grade_min, grade_max, strokes_min, strokes_max);
        card = 1;
        previous_card = 0;
        sub_screen_handler_.SwitchMode (SubScreenMode::CARDS, GameMode::KANJI);
      }

      if (touch.px > TP_CARD1_X1 && touch.px < TP_CARD1_X2 &&
          touch.py > TP_CARD_Y1 && touch.py < TP_CARD_Y2)
        card--;
      if (touch.px > TP_CARD2_X1 && touch.px < TP_CARD2_X2 &&
          touch.py > TP_CARD_Y1 && touch.py < TP_CARD_MIDDLE_Y)
        card++;
      if (touch.px > TP_CARD2_X1 && touch.px < TP_CARD2_X2 &&
        touch.py > TP_CARD_MIDDLE_Y && touch.py < TP_CARD_Y2)
        card = rand()%xmlparser_.QueryResultSize(grade_min, grade_max,
                                                 strokes_min, strokes_max) + 1;

      if (card < 1) card = 1;
      if (card > xmlparser_.QueryResultSize(grade_min, grade_max,
                                            strokes_min, strokes_max))
        card = xmlparser_.QueryResultSize(grade_min, grade_max,
                                          strokes_min, strokes_max);
      if(sy < 0) sy = 0;
      if(sy >= height - 192) sy = height - 1 - 192;

      Card c;
      if (previous_card != card)
      {
        if (previous_card)
          sound_handler_.PlayEffect(SoundHandler::ACTION);
        sy = 0;
        main_screen_handler_.Scroll(0,sy);
        c = xmlparser_.card(card, grade_min, grade_max,
                            strokes_min, strokes_max);
        sub_screen_handler_.PrintCard(c);
        main_screen_handler_.PrintCard(c, romaji_conversion);
        previous_card = card;
      }
      main_screen_handler_.Scroll(0,sy);
      swiWaitForVBlank();
   }
}

void DSMemorizer::KanjiQuizMode ()
{
  xmlparser_.Init("/db/kanjis.xml");

  main_screen_handler_.SwitchMode (MainScreenMode::VERTICAL_TEXTBOXES_VISIBLE,
                                   GameMode::KANJI_QUIZ, 3);
  sub_screen_handler_.SwitchMode (SubScreenMode::KANJI_CHOOSE,
                                  GameMode::KANJI_QUIZ);
  main_screen_handler_.Captions("Translation", "on reading", "kun reading");


  // player score
  int score = 0;
  // Player answers
  int answers = 0;
  // Selected kanji number [1-4]
  int selected_kanji = 0;
  // Vertical scroll
  int sy = 0;

  int height = 256;
  int keys = 0;
  touchPosition touch;

  unsigned grade_min, grade_max, strokes_min, strokes_max;
  // By default: all the kanjis
  grade_min = MIN_GRADE, grade_max = MAX_GRADE;
  strokes_min = MIN_STROKES, strokes_max = MAX_STROKES;

  int card = rand()%xmlparser_.QueryResultSize(grade_min, grade_max,
                                               strokes_min, strokes_max) + 1;
  int selected_card [4];
  // Correct kanji position [1-4]
  int correct = randomize_positions (card,
                  xmlparser_.QueryResultSize(grade_min, grade_max,
                                          strokes_min, strokes_max),
                  selected_card[0], selected_card[1],
                  selected_card[2], selected_card[3]);
  Card c;
  c = xmlparser_.card (card, grade_min, grade_max,
                       strokes_min, strokes_max);
  main_screen_handler_.PrintCard(c, romaji_conversion);

  sub_screen_handler_.PrintScreen(
          xmlparser_.card(selected_card[0], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          xmlparser_.card(selected_card[1], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          xmlparser_.card(selected_card[2], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          xmlparser_.card(selected_card[3], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          score, answers);

  // Loop
  bool done = false;
  while(!(keys & KEY_B) && !done)
  {
      scanKeys();

      touchRead(&touch);

      keys = keysDown();

      if ((keys & KEY_A) || (keys & KEY_Y) || (keys & KEY_X))
        sound_handler_.PlayEffect(SoundHandler::ACTION);

      if (keysHeld() & KEY_UP) sy--;
      if (keysHeld() & KEY_DOWN) sy++;
      if (keys & KEY_Y)
      {
        SetOptions(grade_min, grade_max, strokes_min, strokes_max);
        sub_screen_handler_.SwitchMode (SubScreenMode::KANJI_CHOOSE,
                                        GameMode::KANJI_QUIZ);
        card = rand()%xmlparser_.QueryResultSize(grade_min, grade_max,
                                               strokes_min, strokes_max) + 1;
        // Correct kanji position [1-4]
        correct = randomize_positions (card,
                     xmlparser_.QueryResultSize(grade_min, grade_max,
                                                strokes_min, strokes_max),
                     selected_card[0], selected_card[1],
                     selected_card[2], selected_card[3]);

        c = xmlparser_.card (card, grade_min, grade_max,
                             strokes_min, strokes_max);
        main_screen_handler_.PrintCard(c, romaji_conversion);

        sub_screen_handler_.PrintScreen(
                xmlparser_.card(selected_card[0], grade_min, grade_max,
                                strokes_min, strokes_max).symbol(),
                xmlparser_.card(selected_card[1], grade_min, grade_max,
                                strokes_min, strokes_max).symbol(),
                xmlparser_.card(selected_card[2], grade_min, grade_max,
                                strokes_min, strokes_max).symbol(),
                xmlparser_.card(selected_card[3], grade_min, grade_max,
                                strokes_min, strokes_max).symbol(),
                score, answers);
      }

      if (touch.px > TP_LEFT_BOTTOM_IMAGE_X1 &&
          touch.px < TP_LEFT_BOTTOM_IMAGE_X2 &&
          touch.py > TP_LEFT_BOTTOM_IMAGE_Y1 &&
          touch.py < TP_LEFT_BOTTOM_IMAGE_Y2)
        done = true;

      if (touch.px > TP_RIGHT_BOTTOM_IMAGE_X1 &&
          touch.px < TP_RIGHT_BOTTOM_IMAGE_X2 &&
          touch.py > TP_RIGHT_BOTTOM_IMAGE_Y1 &&
          touch.py < TP_RIGHT_BOTTOM_IMAGE_Y2)
      {
        SetOptions(grade_min, grade_max, strokes_min, strokes_max);
        sub_screen_handler_.SwitchMode (SubScreenMode::KANJI_CHOOSE,
                                        GameMode::KANJI_QUIZ);
        card = rand()%xmlparser_.QueryResultSize(grade_min, grade_max,
                                                 strokes_min, strokes_max) + 1;
        // Correct kanji position [1-4]
        correct = randomize_positions (card,
                                       xmlparser_.QueryResultSize(grade_min,
                                          grade_max, strokes_min, strokes_max),
                                       selected_card[0], selected_card[1],
                                       selected_card[2], selected_card[3]);
        Card c;
        c = xmlparser_.card(card, grade_min, grade_max,
                            strokes_min, strokes_max);
        main_screen_handler_.PrintCard(c, romaji_conversion);

        sub_screen_handler_.PrintScreen(
          xmlparser_.card(selected_card[0], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          xmlparser_.card(selected_card[1], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          xmlparser_.card(selected_card[2], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          xmlparser_.card(selected_card[3], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          score, answers);
      }

      if (touch.px > KC_BOX1_X &&
          touch.px < (KC_BOX1_X + (KC_NORMAL_SIZE * 3 >> 1)) &&
          touch.py > KC_BOX1_Y &&
          touch.py < (KC_BOX1_Y + (KC_NORMAL_SIZE * 3 >> 1)))
        selected_kanji = 1;
      if (touch.px > KC_BOX2_X &&
          touch.px < (KC_BOX2_X + (KC_NORMAL_SIZE * 3 >> 1)) &&
          touch.py > KC_BOX2_Y &&
          touch.py < (KC_BOX2_Y + (KC_NORMAL_SIZE * 3 >> 1)))
        selected_kanji = 2;
      if (touch.px > KC_BOX3_X &&
          touch.px < (KC_BOX3_X + (KC_NORMAL_SIZE * 3 >> 1)) &&
          touch.py > KC_BOX3_Y &&
          touch.py < (KC_BOX3_Y + (KC_NORMAL_SIZE * 3 >> 1)))
        selected_kanji = 3;
      if (touch.px > KC_BOX4_X &&
          touch.px < (KC_BOX4_X + (KC_NORMAL_SIZE * 3 >> 1)) &&
          touch.py > KC_BOX4_Y &&
          touch.py < (KC_BOX4_Y + (KC_NORMAL_SIZE * 3 >> 1)))
        selected_kanji = 4;

      if (sy < 0) sy = 0;
      if (sy >= height - 192) sy = height - 1 - 192;

      if (selected_kanji)
      {
        sound_handler_.PlayEffect(SoundHandler::ACTION);
        ++answers;
        if (selected_kanji == correct)
        {
          ++score;
          sub_screen_handler_.PrintBoards (score, answers);
          sub_screen_handler_.PrintTick (selected_kanji);
        }
        else
        {
          sub_screen_handler_.PrintTick (correct);
          sub_screen_handler_.PrintCross (selected_kanji);
        }

        do
        {
          scanKeys();
        }while(!keysDown());

        sy = 0;
        main_screen_handler_.Scroll(0,sy);

        card = rand()%xmlparser_.QueryResultSize(grade_min, grade_max,
                                                 strokes_min, strokes_max) + 1;
        correct = randomize_positions (card,
                                       xmlparser_.QueryResultSize(grade_min, grade_max,
                                               strokes_min, strokes_max),
                                       selected_card[0], selected_card[1],
                                       selected_card[2], selected_card[3]);
        c = xmlparser_.card(card, grade_min, grade_max, strokes_min, strokes_max);
        main_screen_handler_.PrintCard(c, romaji_conversion);

        sub_screen_handler_.PrintScreen(
          xmlparser_.card(selected_card[0], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          xmlparser_.card(selected_card[1], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          xmlparser_.card(selected_card[2], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          xmlparser_.card(selected_card[3], grade_min, grade_max,
                          strokes_min, strokes_max).symbol(),
          score, answers);

        selected_kanji = 0;
      }
      main_screen_handler_.Scroll(0,sy);
      swiWaitForVBlank();
   }
}


void DSMemorizer::VocabularyMode ()
{
  xmlparser_.Init("/db/vocabulary.xml");

  main_screen_handler_.SwitchMode (MainScreenMode::VERTICAL_TEXTBOXES,
                                   GameMode::VOCABULARY, 3);
  sub_screen_handler_.SwitchMode (SubScreenMode::CARDS, GameMode::VOCABULARY);
  main_screen_handler_.Captions("Kanji", "Reading", "Translation");

  int sy = 0;
  // Shown card
  int card = 1;
  // Previously shown card
  int previous_card = 0;

  int height = 256;
  int keys = 0;
  touchPosition touch;
  // Loop
  bool done = false;
  while(!(keys & KEY_B) && !done)
  {
      scanKeys();

      touchRead(&touch);

     keys = keysDown();

      if ((keys & KEY_LEFT) || (keys & KEY_RIGHT) || (keys & KEY_A) ||
          (keys & KEY_X) || (keys & KEY_Y))
        sound_handler_.PlayEffect(SoundHandler::ACTION);

      if (keysHeld() & KEY_UP) sy--;
      if (keysHeld() & KEY_DOWN) sy++;
      if (keys & KEY_LEFT) card--;
      if (keys & KEY_RIGHT) card++;
      if (keys & KEY_A)
        if (main_screen_handler_.ViewNext())
          ++card;
      if (keys & KEY_X)
        card = rand()%xmlparser_.package_records() + 1;
      if (keys & KEY_Y)
      {
        SetOptions();
        sub_screen_handler_.SwitchMode (SubScreenMode::CARDS,
                                        GameMode::VOCABULARY);
      }

      if (touch.px > TP_LEFT_BOTTOM_IMAGE_X1 &&
          touch.px < TP_LEFT_BOTTOM_IMAGE_X2 &&
          touch.py > TP_LEFT_BOTTOM_IMAGE_Y1 &&
          touch.py < TP_LEFT_BOTTOM_IMAGE_Y2)
        done = true;

      if (touch.px > TP_RIGHT_BOTTOM_IMAGE_X1 &&
          touch.px < TP_RIGHT_BOTTOM_IMAGE_X2 &&
          touch.py > TP_RIGHT_BOTTOM_IMAGE_Y1 &&
          touch.py < TP_RIGHT_BOTTOM_IMAGE_Y2)
      {
        SetOptions();
        sub_screen_handler_.SwitchMode (SubScreenMode::CARDS,
                                        GameMode::VOCABULARY);
      }

      if (touch.px > TP_CARD1_X1 && touch.px < TP_CARD1_X2 &&
          touch.py > TP_CARD_Y1 && touch.py < TP_CARD_Y2)
        card--;
      if (touch.px > TP_CARD2_X1 && touch.px < TP_CARD2_X2 &&
          touch.py > TP_CARD_Y1 && touch.py < TP_CARD_MIDDLE_Y)
        card++;
      if (touch.px > TP_CARD2_X1 && touch.px < TP_CARD2_X2 &&
          touch.py > TP_CARD_MIDDLE_Y && touch.py < TP_CARD_Y2)
        card = rand()%xmlparser_.package_records() + 1;

      if (card < 1) card = 1;
      if (card > xmlparser_.package_records())
        card = xmlparser_.package_records();
      if(sy < 0) sy = 0;
      if(sy >= height - 192) sy = height - 1 - 192;

      if (previous_card != card)
      {
        if (previous_card)
          sound_handler_.PlayEffect(SoundHandler::ACTION);
        sy = 0;
        Card c;
        c = xmlparser_.card(card);
        sub_screen_handler_.PrintCard(c);
        main_screen_handler_.Scroll (0, sy);
        main_screen_handler_.PrintCard(c, romaji_conversion);
        previous_card = card;
      }
      main_screen_handler_.Scroll (0,sy);
      swiWaitForVBlank();
   }
}

void DSMemorizer::VocabularyQuizMode ()
{
  xmlparser_.Init("/db/vocabulary.xml");

  main_screen_handler_.SwitchMode (MainScreenMode::VERTICAL_TEXTBOXES_VISIBLE,
                                   GameMode::VOCABULARY_QUIZ, 2);
  sub_screen_handler_.SwitchMode (SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE,
                                  GameMode::VOCABULARY_QUIZ);
  main_screen_handler_.Captions("Translation", "Reading", "");

  // player score
  int score = 0;
  // Player answers
  int answers = 0;
  // Selected kanji number [1-4]
  int selected_kanji = 0;
  // Vertical scroll
  int sy = 0;

  int height = 256;
  int keys = 0;
  touchPosition touch;


  int card_number = rand()%xmlparser_.package_records() + 1;
  int selected_card [4];
  // Correct kanji position [1-4]
  int correct = randomize_positions (card_number,
                                     xmlparser_.package_records(),
                                     selected_card[0], selected_card[1],
                                     selected_card[2], selected_card[3]);
  Card c;
  c = xmlparser_.card(card_number);
  main_screen_handler_.PrintCard(c, romaji_conversion);

  sub_screen_handler_.PrintScreen(xmlparser_.card(selected_card[0]).symbol(),
                                  xmlparser_.card(selected_card[1]).symbol(),
                                  xmlparser_.card(selected_card[2]).symbol(),
                                  xmlparser_.card(selected_card[3]).symbol(),
                                  score, answers);

  // Loop
  bool done = false;
  while(!(keys & KEY_B) && !done)
  {
      scanKeys();

      touchRead(&touch);

      keys = keysDown();

      if ((keys & KEY_LEFT) || (keys & KEY_RIGHT) || (keys & KEY_A) ||
          (keys & KEY_X) || (keys & KEY_Y))
        sound_handler_.PlayEffect(SoundHandler::ACTION);

      if (keysHeld() & KEY_UP) sy--;
      if (keysHeld() & KEY_DOWN) sy++;
      if (keys & KEY_Y)
      {
        SetOptions();
        sub_screen_handler_.SwitchMode (SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE,
                                        GameMode::VOCABULARY_QUIZ);
        sub_screen_handler_.PrintScreen(
          xmlparser_.card(selected_card[0]).symbol(),
          xmlparser_.card(selected_card[1]).symbol(),
          xmlparser_.card(selected_card[2]).symbol(),
          xmlparser_.card(selected_card[3]).symbol(),
          score, answers);
      }

      if (touch.px > TP_LEFT_BOTTOM_IMAGE_X1 &&
          touch.px < TP_LEFT_BOTTOM_IMAGE_X2 &&
          touch.py > TP_LEFT_BOTTOM_IMAGE_Y1 &&
          touch.py < TP_LEFT_BOTTOM_IMAGE_Y2)
        done = true;

      if (touch.px > TP_RIGHT_BOTTOM_IMAGE_X1 &&
          touch.px < TP_RIGHT_BOTTOM_IMAGE_X2 &&
          touch.py > TP_RIGHT_BOTTOM_IMAGE_Y1 &&
          touch.py < TP_RIGHT_BOTTOM_IMAGE_Y2)
      {
        SetOptions();
        sub_screen_handler_.SwitchMode (SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE,
                                        GameMode::VOCABULARY_QUIZ);
        sub_screen_handler_.PrintScreen(
          xmlparser_.card(selected_card[0]).symbol(),
          xmlparser_.card(selected_card[1]).symbol(),
          xmlparser_.card(selected_card[2]).symbol(),
          xmlparser_.card(selected_card[3]).symbol(),
          score, answers);
      }

      if (touch.px > VTBC_BOX1_X &&
          touch.px < (SCREEN_WIDTH - VTBC_BOX1_X) &&
          touch.py > VTBC_BOX1_Y &&
          touch.py < (VTBC_BOX1_Y + (VTBC_NORMAL_SIZE * 3 >> 1)))
        selected_kanji = 1;
      if (touch.px > VTBC_BOX2_X &&
          touch.px < (SCREEN_WIDTH - VTBC_BOX2_X) &&
          touch.py > VTBC_BOX2_Y &&
          touch.py < (VTBC_BOX2_Y + (VTBC_NORMAL_SIZE * 3 >> 1)))
        selected_kanji = 2;
      if (touch.px > VTBC_BOX3_X &&
          touch.px < (SCREEN_WIDTH - VTBC_BOX3_X) &&
          touch.py > VTBC_BOX3_Y &&
          touch.py < (VTBC_BOX3_Y + (VTBC_NORMAL_SIZE * 3 >> 1)))
        selected_kanji = 3;
      if (touch.px > VTBC_BOX4_X &&
          touch.px < (SCREEN_WIDTH - VTBC_BOX4_X) &&
          touch.py > VTBC_BOX4_Y &&
          touch.py < (VTBC_BOX4_Y + (VTBC_NORMAL_SIZE * 3 >> 1)))
        selected_kanji = 4;

      if (sy < 0) sy = 0;
      if (sy >= height - 192) sy = height - 1 - 192;

      if (selected_kanji)
      {
        sound_handler_.PlayEffect(SoundHandler::ACTION);
        ++answers;
        if (selected_kanji == correct)
        {
          ++score;
          sub_screen_handler_.PrintBoards (score, answers);
          sub_screen_handler_.PrintTick (selected_kanji);
        }
        else
        {
          sub_screen_handler_.PrintTick (correct);
          sub_screen_handler_.PrintCross (selected_kanji);
        }

        do
        {
          scanKeys();
        }while(!keysDown());

        sy = 0;
        main_screen_handler_.Scroll(0,sy);

        card_number = rand()%xmlparser_.package_records() + 1;
        correct = randomize_positions (card_number,
                                       xmlparser_.package_records(),
                                       selected_card[0], selected_card[1],
                                       selected_card[2], selected_card[3]);
        c = xmlparser_.card(card_number);
        main_screen_handler_.PrintCard(c, romaji_conversion);

        sub_screen_handler_.PrintScreen(
          xmlparser_.card(selected_card[0]).symbol(),
          xmlparser_.card(selected_card[1]).symbol(),
          xmlparser_.card(selected_card[2]).symbol(),
          xmlparser_.card(selected_card[3]).symbol(),
          score, answers);

        selected_kanji = 0;
      }
      main_screen_handler_.Scroll(0,sy);
      swiWaitForVBlank();
   }
}

void DSMemorizer::SetOptions (unsigned& grade_min, unsigned& grade_max,
                              unsigned& strokes_min, unsigned& strokes_max)
{
  sub_screen_handler_.SwitchMode (SubScreenMode::OPTIONS_GRADE_STROKES_ROMAJI,
                                  GameMode::NONE);
  Graphics::SetColors();
  unsigned prev_grade_min, prev_grade_max, prev_strokes_min, prev_strokes_max;
  prev_grade_min = prev_grade_max = prev_strokes_min = prev_strokes_max = 0;
  bool prev_romaji_conversion = false;

  int keys = 0;
  touchPosition touch;

  // Loop
  bool done = false;
  while(!(keys & KEY_B) && !done)
  {
    scanKeys();

    touchRead(&touch);

    keys = keysDown();

    if (touch.px > TP_RIGHT_BOTTOM_IMAGE_X1 &&
        touch.px < TP_RIGHT_BOTTOM_IMAGE_X2 &&
        touch.py > TP_RIGHT_BOTTOM_IMAGE_Y1 &&
        touch.py < TP_RIGHT_BOTTOM_IMAGE_Y2)
      done = true;

    if (touch.px > TP_OGSR_MINUS1_X1 && touch.px < TP_OGSR_MINUS1_X2 &&
        touch.py > TP_OGSR_GRADE_Y1 && touch.py < TP_OGSR_GRADE_Y2)
      --grade_min;
    if (touch.px > TP_OGSR_PLUS1_X1 && touch.px < TP_OGSR_PLUS1_X2 &&
        touch.py > TP_OGSR_GRADE_Y1 && touch.py < TP_OGSR_GRADE_Y2)
      ++grade_min;
    if (touch.px > TP_OGSR_MINUS2_X1 && touch.px < TP_OGSR_MINUS2_X2 &&
        touch.py > TP_OGSR_GRADE_Y1 && touch.py < TP_OGSR_GRADE_Y2)
      --grade_max;
    if (touch.px > TP_OGSR_PLUS2_X1 && touch.px < TP_OGSR_PLUS2_X2 &&
        touch.py > TP_OGSR_GRADE_Y1 && touch.py < TP_OGSR_GRADE_Y2)
      ++grade_max;

    if (touch.px > TP_OGSR_MINUS1_X1 && touch.px < TP_OGSR_MINUS1_X2 &&
        touch.py > TP_OGSR_STROKES_Y1 && touch.py < TP_OGSR_STROKES_Y2)
      --strokes_min;
    if (touch.px > TP_OGSR_PLUS1_X1 && touch.px < TP_OGSR_PLUS1_X2 &&
        touch.py > TP_OGSR_STROKES_Y1 && touch.py < TP_OGSR_STROKES_Y2)
      ++strokes_min;
    if (touch.px > TP_OGSR_MINUS2_X1 && touch.px < TP_OGSR_MINUS2_X2 &&
        touch.py > TP_OGSR_STROKES_Y1 && touch.py < TP_OGSR_STROKES_Y2)
      --strokes_max;
    if (touch.px > TP_OGSR_PLUS2_X1 && touch.px < TP_OGSR_PLUS2_X2 &&
        touch.py > TP_OGSR_STROKES_Y1 && touch.py < TP_OGSR_STROKES_Y2)
      ++strokes_max;
    if (touch.px > TP_OGSR_ROMAJI_CHECKBOX_X1 &&
        touch.px < TP_OGSR_ROMAJI_CHECKBOX_X2 &&
        touch.py > TP_OGSR_ROMAJI_CHECKBOX_Y1 &&
        touch.py < TP_OGSR_ROMAJI_CHECKBOX_Y2)
      romaji_conversion = !romaji_conversion;

    if (grade_min < MIN_GRADE) grade_min = MIN_GRADE;
    if (grade_min > MAX_GRADE) grade_min = MAX_GRADE;
    if (grade_max > MAX_GRADE) grade_max = MAX_GRADE;
    if (grade_max < MIN_GRADE) grade_max = MIN_GRADE;
    if (strokes_min < MIN_STROKES) strokes_min = MIN_STROKES;
    if (strokes_min > MAX_STROKES) strokes_min = MAX_STROKES;
    if (strokes_max > MAX_STROKES) strokes_max = MAX_STROKES;
    if (strokes_max < MIN_STROKES) strokes_max = MIN_STROKES;

    if (prev_grade_min != grade_min || prev_grade_max != grade_max ||
        prev_strokes_min != strokes_min || prev_strokes_max != strokes_max ||
        prev_romaji_conversion != romaji_conversion)
    {
      sound_handler_.PlayEffect(SoundHandler::ACTION);
      sub_screen_handler_.PrintOptions(grade_min, grade_max,
                                       strokes_min, strokes_max,
                                       romaji_conversion);

      prev_grade_min = grade_min, prev_grade_max = grade_max;
      prev_strokes_min = strokes_min, prev_strokes_max = strokes_max;
      prev_romaji_conversion = romaji_conversion;
      swiWaitForVBlank();
      swiWaitForVBlank();
    }
    swiWaitForVBlank();
  }
  sound_handler_.PlayEffect(SoundHandler::ACTION);
}

void DSMemorizer::SetOptions ()
{
  sub_screen_handler_.SwitchMode (SubScreenMode::OPTIONS_ROMAJI,
                                  GameMode::NONE);
  Graphics::SetColors();
  bool prev_romaji_conversion = false;

  int keys = 0;
  touchPosition touch;

  // Loop
  bool done = false;
  while(!(keys & KEY_B) && !done)
  {
    scanKeys();

    touchRead(&touch);

    keys = keysDown();

    if (touch.px > TP_RIGHT_BOTTOM_IMAGE_X1 &&
        touch.px < TP_RIGHT_BOTTOM_IMAGE_X2 &&
        touch.py > TP_RIGHT_BOTTOM_IMAGE_Y1 &&
        touch.py < TP_RIGHT_BOTTOM_IMAGE_Y2)
      done = true;

    if (touch.px > TP_OR_ROMAJI_CHECKBOX_X1 &&
        touch.px < TP_OR_ROMAJI_CHECKBOX_X2 &&
        touch.py > TP_OR_ROMAJI_CHECKBOX_Y1 &&
        touch.py < TP_OR_ROMAJI_CHECKBOX_Y2)
      romaji_conversion = !romaji_conversion;

    if (prev_romaji_conversion != romaji_conversion)
    {
      sound_handler_.PlayEffect(SoundHandler::ACTION);
      sub_screen_handler_.PrintOptions(romaji_conversion);

      prev_romaji_conversion = romaji_conversion;
      swiWaitForVBlank();
      swiWaitForVBlank();
    }
    swiWaitForVBlank();
  }
  sound_handler_.PlayEffect(SoundHandler::ACTION);
}

