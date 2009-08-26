/// \file kanjimode.cpp Kanji game mode.
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
#include "textbox.h"
#include "textboxhandler.h"
#include "card.h"
#include "xmlparser.h"
#include "kanjimode.h"

#include "kanjibg.h"
#include "kanjisubbg.h"

void KanjiMode::Init(int bgid)
{
  dmaCopy(kanjibgBitmap, bgGetGfxPtr(bgid), 256*256);
  dmaCopy(kanjibgPal, BG_PALETTE, 256*2);
  BG_PALETTE[Types::Color::BLACK]= RGB15(0,0,0);
  BG_PALETTE[Types::Color::GREY]= RGB15(15,15,15);
  BG_PALETTE[Types::Color::WHITE]= RGB15(18,18,28);

  videoSetModeSub (MODE_5_2D);
  int subbg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
  dmaCopy(kanjisubbgBitmap, bgGetGfxPtr(subbg3), 256*256);
  dmaCopy(kanjisubbgPal, BG_PALETTE_SUB, 256*2);
  int subbg2 = bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0,0);
  BG_PALETTE_SUB[Types::Color::BLACK]= RGB15(0,0,0);
  BG_PALETTE_SUB[Types::Color::GREY]= RGB15(15,15,15);
  BG_PALETTE_SUB[Types::Color::WHITE]= RGB15(18,18,28);

  xmlparser_ = new XMLParser ();
  xmlparser_->Init("/db/kanjis.xml");
  int bg2 = bgInit(2, BgType_Bmp8, BgSize_B8_256x256, 0,0);

  tbh_ = new TextBoxHandler ();
  tbh_->Init();

  // Main screen
  caption_kanji_ = tbh_->NewTextBox (bg2, Types::VERA_FONT, 8,15,17,60,0);
  kanji_ = tbh_->NewTextBox (bg2, Types::MONA_FONT, 30,12,60,100,0);
  kanji_->floats(true);
  caption_first_reading_ = tbh_->NewTextBox (bg2, Types::VERA_FONT, 8,70,17,100,0);
  first_reading_ = tbh_->NewTextBox (bg2, Types::MONA_FONT, 10,70,30,100,0);
  caption_second_reading_ = tbh_->NewTextBox (bg2, Types::VERA_FONT, 8,70,35,100,0);
  second_reading_ = tbh_->NewTextBox (bg2, Types::MONA_FONT, 10,70,40,100,0);
  caption_translation_ = tbh_->NewTextBox (bg2, Types::VERA_FONT, 8,10,100,100,0);
  translation_ = tbh_->NewTextBox (bg2, Types::MONA_FONT, 10,10,100,235,0);
  caption_example_ = tbh_->NewTextBox (bg2, Types::VERA_FONT, 8,10,130,100,0);
  example_kanji_ = tbh_->NewTextBox (bg2, Types::MONA_FONT, 10,10,135,235,0);
  example_reading_ = tbh_->NewTextBox (bg2, Types::MONA_FONT, 10,10,160,235,0);
  example_translation_ = tbh_->NewTextBox (bg2, Types::MONA_FONT, 10,10,190,235,0);

  // Sub screen
  card_number_ = tbh_->NewTextBox (subbg3, Types::VERA_FONT, 10,20,10,225,0);


  int sy = 0;
  int card = 1;
  int previous_card = 0;
  int height = 256;
  int keys = 0;
  touchPosition tp = {0,0,0,0};
  // Loop
  while(!(keys & KEY_B))
   {
      scanKeys();

      touchRead(&tp);

      keys = keysHeld();

      if(keys & KEY_UP) sy--;
      if(keys & KEY_DOWN) sy++;
      if(keys & KEY_LEFT) card--;
      if(keys & KEY_RIGHT) card++;

      //if (keys & KEY_TOUCH){
        //iprintf("hola");
        if (tp.px > 36 && tp.px < 102 && tp.py > 40 && tp.py < 146)
          card--;
        if (tp.px > 156 && tp.px < 224 && tp.py > 40 && tp.py < 146){
          card++;
          iprintf("hola");
        }
      //}

      if (card < 1) card = 1;
      if (card > xmlparser_->package_records())
        card = xmlparser_->package_records();
      if(sy < 0) sy = 0;
      if(sy >= height - 192) sy = height - 1 - 192;

      swiWaitForVBlank();
      Card c;
      if (previous_card != card)
      {
        sy = 0;
        dmaCopy(kanjibgBitmap, bgGetGfxPtr(bgid), 256*256);
        bgSetScroll(bgid,0,sy);
        bgSetScroll(bg2,0,sy);
        bgUpdate();
        c = xmlparser_->card(card);
        PrintCard(c);
        previous_card = card;
      }
      bgSetScroll(bgid,0,sy);
      bgSetScroll(bg2,0,sy);
      bgUpdate();
   }

}

void KanjiMode::PrintCard (const Card& card)
{
 	kanji_->text(card.symbol());
	first_reading_->text(card.reading());
	second_reading_->text(card.reading2());
	translation_->text(card.translation());
	example_kanji_->text(card.example_symbol());
	example_reading_->text(card.example_reading());
	example_translation_->text(card.example_translation());
	caption_kanji_->text("Kanji");
	caption_first_reading_->text("First reading");
	caption_second_reading_->text("Second reading");
	caption_translation_->text("Translation");
	caption_example_->text("Example");

  char* card_number_text = new char [40];
  sprintf(card_number_text, "Card number: %i",card.index());
	card_number_->text("asdfasdfadsf");
	tbh_->PrintAll();
}

KanjiMode::~KanjiMode ()
{
  // it destroys all its text boxes
  tbh_->~TextBoxHandler();
  delete tbh_;
  xmlparser_->~XMLParser();
  delete xmlparser_;
}

