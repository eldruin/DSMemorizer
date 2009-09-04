/// \file vocabularyquizmode.cpp Vocabulary quiz game mode.
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

#include <string>
#include <nds.h>
#include "types.h"
#include "card.h"
#include "xmlparser.h"
#include "graphics.h"
#include "screenshandler.h"
#include "mainscreenhandler.h"
#include "subscreenhandler.h"
#include "vocabularyquizmode.h"

using namespace Types;

void GameMode::VocabularyQuizMode (MainScreenHandler* main_screen_handler,
                                   SubScreenHandler* sub_screen_handler)
{
  XMLParser xmlparser;
  xmlparser.Init("/db/vocabulary.xml");

  main_screen_handler->SwitchMode (MainScreenMode::VERTICAL_TEXTBOXES_VISIBLE,
                                   GameMode::VOCABULARY_QUIZ, 2);
  sub_screen_handler->SwitchMode (SubScreenMode::VERTICAL_TEXTBOXES_CHOOSE);
  main_screen_handler->Captions("Translation", "Reading", "");
  Graphics::SetColors();

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


  int card_number = rand()%xmlparser.package_records() + 1;
  int selected_card [4];
  // Correct kanji position [1-4]
  int correct = randomize_positions (card_number,
                                     xmlparser.package_records(),
                                     selected_card[0], selected_card[1],
                                     selected_card[2], selected_card[3]);
  Card c;
  c = xmlparser.card(card_number);
  main_screen_handler->PrintCard(c);

  sub_screen_handler->PrintScreen(xmlparser.card(selected_card[0]).symbol(),
                                  xmlparser.card(selected_card[1]).symbol(),
                                  xmlparser.card(selected_card[2]).symbol(),
                                  xmlparser.card(selected_card[3]).symbol(),
                                  score, answers);

  // Loop
  bool done = false;
  while(!(keys & KEY_B) && !done)
  {
      scanKeys();

      touchRead(&touch);

      keys = keysHeld();

      if (keys & KEY_UP) sy--;
      if (keys & KEY_DOWN) sy++;
      if (touch.px > 5 && touch.px < 39 && touch.py > 152 && touch.py < 187)
        done = true;

      if (touch.px > 33 && touch.px < 228 && touch.py > 44 && touch.py < 54)
        selected_kanji = 1;
      if (touch.px > 33 && touch.px < 228 && touch.py > 73 && touch.py < 83)
        selected_kanji = 2;
      if (touch.px > 33 && touch.px < 228 && touch.py > 102 && touch.py < 112)
        selected_kanji = 3;
      if (touch.px > 33 && touch.px < 228 && touch.py > 131 && touch.py < 141)
        selected_kanji = 4;

      if (sy < 0) sy = 0;
      if (sy >= height - 192) sy = height - 1 - 192;

      if (selected_kanji)
      {
        ++answers;
        if (selected_kanji == correct)
        {
          ++score;
          sub_screen_handler->PrintBoards (score, answers);
          sub_screen_handler->PrintTick (selected_kanji);
        }
        else
        {
          sub_screen_handler->PrintTick (correct);
          sub_screen_handler->PrintCross (selected_kanji);
        }

        do
        {
          scanKeys();
        }while(!keysDown());

        sy = 0;
        main_screen_handler->Scroll(0,sy);

        card_number = rand()%xmlparser.package_records() + 1;
        correct = randomize_positions (card_number,
                                       xmlparser.package_records(),
                                       selected_card[0], selected_card[1],
                                       selected_card[2], selected_card[3]);
        c = xmlparser.card(card_number);
        main_screen_handler->PrintCard(c);

        sub_screen_handler->PrintScreen(
          xmlparser.card(selected_card[0]).symbol(),
          xmlparser.card(selected_card[1]).symbol(),
          xmlparser.card(selected_card[2]).symbol(),
          xmlparser.card(selected_card[3]).symbol(),
          score, answers);

        selected_kanji = 0;
      }
      main_screen_handler->Scroll(0,sy);
      swiWaitForVBlank();
   }
}


