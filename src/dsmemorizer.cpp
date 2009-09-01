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
#include <string>
#include "types.h"
#include "screenshandler.h"
#include "subscreenhandler.h"
#include "mainscreenhandler.h"
#include "kanjimode.h"
#include "kanjiquizmode.h"
#include "vocabularymode.h"
#include "vocabularyquizmode.h"

using namespace Types;

void DSMemorizer()
{
  ScreensHandler sh;
  sh.Init();
  MainScreenHandler main_screen_handler (GameMode::SPLASH_SCREEN);
  main_screen_handler.Init (MainScreenMode::SPLASH_SCREEN, &sh);
  SubScreenHandler sub_screen_handler (SubScreenMode::MAIN_MENU);
  sub_screen_handler.Init (&sh);

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

      if (touch.px > 23 && touch.px < 50 && touch.py > 25 && touch.py < 83)
        selected_button = 1;
      if (touch.px > 99 && touch.px < 158 && touch.py > 25 && touch.py < 83)
        selected_button = 2;
      if (touch.px > 174 && touch.px < 233 && touch.py > 25 && touch.py < 83)
        selected_button = 3;
      if (touch.px > 23 && touch.px < 50 && touch.py > 97 && touch.py < 156)
        selected_button = 4;

      if (sy < 0) sy = 0;
      if (sy >= height - 192) sy = height - 1 - 192;

      if (selected_button)
      {
        switch(selected_button)
        {
          case 1: GameMode::KanjiMode();
          break;
          case 2: GameMode::KanjiQuizMode();
          break;
          case 3: GameMode::VocabularyMode();
          break;
          case 4: GameMode::VocabularyQuizMode();
          break;
        }
        main_screen_handler.Show ();
        sub_screen_handler.Show ();
      }
      swiWaitForVBlank();
   }

}
