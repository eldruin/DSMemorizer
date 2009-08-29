/// \file vocabularymode.cpp Vocabulary memorizing game mode.
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
#include "vocabularymode.h"

using namespace Types;

void GameMode::VocabularyMode ()
{
  XMLParser xmlparser;
  xmlparser.Init("/db/kanjis.xml");

  ScreensHandler sh;
  sh.Init();
  MainScreenHandler main_screen_handler (MainScreenMode::VERTICAL_TEXTBOXES);
  main_screen_handler.Init (&sh, 3);
  main_screen_handler.Captions("Kanji", "Reading", "Translation");
  SubScreenHandler sub_screen_handler (SubScreenMode::CARDS);
  sub_screen_handler.Init (&sh);

  int sy = 0;
  // Shown card
  int card = 1;
  // Previously shown card
  int previous_card = 0;

  int height = 256;
  int keys = 0;
  touchPosition touch;
  // Loop
  while(!(keys & KEY_B))
  {
      scanKeys();

      touchRead(&touch);

      keys = keysHeld();

      if (keys & KEY_UP) sy--;
      if (keys & KEY_DOWN) sy++;
      if (keys & KEY_LEFT) card--;
      if (keys & KEY_RIGHT) card++;
      if (keys & KEY_A)
        if (main_screen_handler.ViewNext())
          ++card;

      // WARNING: This is related to the image displayed in the sub screen and
      // needs to be changed if the image changes
      if (touch.px > 36 && touch.px < 102 && touch.py > 40 && touch.py < 146)
        card--;
      if (touch.px > 156 && touch.px < 224 && touch.py > 40 && touch.py < 146)
        card++;

      if (card < 1) card = 1;
      if (card > xmlparser.package_records())
        card = xmlparser.package_records();
      if(sy < 0) sy = 0;
      if(sy >= height - 192) sy = height - 1 - 192;

      if (previous_card != card)
      {
        sy = 0;
        Card c;
        c = xmlparser.card(card);
        sub_screen_handler.PrintCard(c);
        main_screen_handler.Scroll (0, sy);
        main_screen_handler.PrintCard(c);
        previous_card = card;
      }
      main_screen_handler.Scroll (0,sy);
      swiWaitForVBlank();
   }
}

