/// \file kanjiquizmode.cpp Kanji quiz game mode.
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
#include <stdio.h>
#include "types.h"
#include "card.h"
#include "xmlparser.h"
#include "screenshandler.h"
#include "mainscreenhandler.h"
#include "subscreenhandler.h"
#include "kanjiquizmode.h"

using namespace Types;

/// Calculates 4 random cards including the correct one given by card_index.
/// \return The correct card position
int randomize_positions (int card_index, int package_records, int& card0,
                         int& card1, int& card2, int& card3);

void GameMode::KanjiQuizMode ()
{
  XMLParser xmlparser;
  xmlparser.Init("/db/kanjis.xml");

  ScreensHandler sh;
  sh.Init();
  MainScreenHandler main_screen_handler (
    MainScreenMode::VERTICAL_TEXTBOXES_VISIBLE);
  main_screen_handler.Init (&sh);
  main_screen_handler.Captions("Translation", "on reading", "kun reading");
  SubScreenHandler sub_screen_handler (SubScreenMode::KANJI_CHOOSE);
  sub_screen_handler.Init (&sh);

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
  main_screen_handler.PrintCard(c);

  sub_screen_handler.PrintScreen(xmlparser.card(selected_card[0]).symbol(),
                                 xmlparser.card(selected_card[1]).symbol(),
                                 xmlparser.card(selected_card[2]).symbol(),
                                 xmlparser.card(selected_card[3]).symbol(),
                                 score, answers);

  // Loop
  while(!(keys & KEY_B))
  {
      scanKeys();

      touchRead(&touch);

      keys = keysHeld();

      if (keys & KEY_UP) sy--;
      if (keys & KEY_DOWN) sy++;

      if (touch.px > 33 && touch.px < 63 && touch.py > 50 && touch.py < 80)
        selected_kanji = 1;
      if (touch.px > 85 && touch.px < 115 && touch.py > 50 && touch.py < 80)
        selected_kanji = 2;
      if (touch.px > 137 && touch.px < 167 && touch.py > 50 && touch.py < 80)
        selected_kanji = 3;
      if (touch.px > 189 && touch.px < 219 && touch.py > 50 && touch.py < 80)
        selected_kanji = 4;

      if (sy < 0) sy = 0;
      if (sy >= height - 192) sy = height - 1 - 192;

      if (selected_kanji)
      {
        ++answers;
        if (selected_kanji == correct)
        {
          ++score;
          sub_screen_handler.PrintBoards (score, answers);
          sub_screen_handler.PrintTick (selected_kanji);
        }
        else
        {
          sub_screen_handler.PrintTick (correct);
          sub_screen_handler.PrintCross (selected_kanji);
        }

        do
        {
          scanKeys();
        }while(!keysDown());

        sy = 0;
        main_screen_handler.Scroll(0,sy);

        card_number = rand()%xmlparser.package_records() + 1;
        correct = randomize_positions (card_number,
                                       xmlparser.package_records(),
                                       selected_card[0], selected_card[1],
                                       selected_card[2], selected_card[3]);
        c = xmlparser.card(card_number);
        main_screen_handler.PrintCard(c);

        sub_screen_handler.PrintScreen(
          xmlparser.card(selected_card[0]).symbol(),
          xmlparser.card(selected_card[1]).symbol(),
          xmlparser.card(selected_card[2]).symbol(),
          xmlparser.card(selected_card[3]).symbol(),
          score, answers);

        selected_kanji = 0;
      }
      main_screen_handler.Scroll(0,sy);
      swiWaitForVBlank();
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
