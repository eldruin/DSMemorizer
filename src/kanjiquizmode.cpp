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
#include "textbox.h"
#include "textboxhandler.h"
#include "card.h"
#include "xmlparser.h"
#include "kanjiquizmode.h"

#include "kanjibg.h"
#include "kanjiquizsubbg.h"
#include "tick.h"
#include "cross.h"

#define SUB 1

inline int x_position (int selected_kanji)
{
  int x_pos;
  switch (selected_kanji)
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

void KanjiQuizMode::Init (int bgid)
{
  dmaCopy(kanjibgBitmap, bgGetGfxPtr(bgid), 256*256);
  dmaCopy(kanjibgPal, BG_PALETTE, 256*2);
  BG_PALETTE[Types::Color::BLACK]= RGB15(0,0,0);
  BG_PALETTE[Types::Color::GREY]= RGB15(15,15,15);
  BG_PALETTE[Types::Color::WHITE]= RGB15(18,18,28);
#if !SUB
  consoleDemoInit();
#endif
#if SUB
  videoSetModeSub(MODE_5_2D);
  vramSetBankC(VRAM_C_SUB_BG);
#endif
  int bg2 = bgInit(2, BgType_Bmp8, BgSize_B8_256x256, 0,0);
#if SUB
  int subbg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

  dmaCopy(kanjiquizsubbgBitmap, bgGetGfxPtr(subbg3), 256*256);
  dmaCopy(kanjiquizsubbgPal, BG_PALETTE_SUB, 256*2);
  dmaCopy(tickPal,BG_PALETTE_SUB+8,43*2);
  dmaCopy(tickPal,BG_PALETTE_SUB+94,43*2);

  BG_PALETTE_SUB[Types::Color::BLACK]= RGB15(0,0,0);
  BG_PALETTE_SUB[Types::Color::GREY]= RGB15(15,15,15);
  BG_PALETTE_SUB[Types::Color::WHITE]= RGB15(28,28,20);
#endif
  xmlparser_ = new XMLParser ();
  xmlparser_->Init("/db/kanjis.xml");

  tbh_ = new TextBoxHandler ();
  tbh_->Init();

  // Main screen
  caption_translation_ = tbh_->NewTextBox (bg2, Types::VERA_FONT, 8,20,30,225,0);
  translation_ = tbh_->NewTextBox (bg2, Types::MONA_FONT, 10,20,40,225,0);
  caption_on_reading_ = tbh_->NewTextBox (bg2, Types::VERA_FONT, 8,20,50,225,0);
  on_reading_ = tbh_->NewTextBox (bg2, Types::MONA_FONT, 10,20,60,225,0);
  caption_kun_reading_ = tbh_->NewTextBox (bg2, Types::VERA_FONT, 8,20,70,225,0);
  kun_reading_ = tbh_->NewTextBox (bg2, Types::MONA_FONT, 10,20,80,225,0);
#if SUB
  // Sub screen
  kanji1_ = tbh_->NewTextBox (subbg3, Types::MONA_FONT, 30,33,80,225,0);
  kanji2_ = tbh_->NewTextBox (subbg3, Types::MONA_FONT, 30,85,80,225,0);
  kanji3_ = tbh_->NewTextBox (subbg3, Types::MONA_FONT, 30,137,80,225,0);
  kanji4_ = tbh_->NewTextBox (subbg3, Types::MONA_FONT, 30,189,80,225,0);
  kanji1_->floats(true);
  kanji2_->floats(true);
  kanji3_->floats(true);
  kanji4_->floats(true);
#endif
  // Text that won't change
  caption_translation_->text("Translation");
  caption_on_reading_->text("on reading");
  caption_kun_reading_->text("kun reading");

  int card_number = rand()%xmlparser_->package_records() + 1;
  Card card = xmlparser_->card(card_number);
  correct_ = PrintScreens(card);

  int selected_kanji = 0;
  int sy = 0;
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

#if !SUB
      iprintf("touch x: %i, y: %i\n",touch.px,touch.py);
#endif

      if (selected_kanji)
      {
        if (selected_kanji == correct_)
        {
#if SUB
          PrintBitmap(x_position(selected_kanji), 103, subbg3, tickBitmap,8);
#endif
        }
        else
        {
#if SUB
          PrintBitmap(x_position(selected_kanji), 103, subbg3, crossBitmap,94);
          PrintBitmap(x_position(correct_), 103, subbg3, tickBitmap,8);
#endif
        }

        do
        {
          scanKeys();
        }while(!keysDown());

        sy = 0;
        dmaCopy(kanjibgBitmap, bgGetGfxPtr(bgid), 256*256);
#if SUB
        dmaCopy(kanjiquizsubbgBitmap, bgGetGfxPtr(subbg3), 256*256);
#endif
        bgSetScroll(bgid,0,sy);
        bgSetScroll(bg2,0,sy);
        bgUpdate();
        card_number = rand()%xmlparser_->package_records() + 1;
        card = xmlparser_->card(card_number);
        correct_ = PrintScreens(card);
        selected_kanji = 0;
      }
      bgSetScroll(bgid,0,sy);
      bgSetScroll(bg2,0,sy);
      bgUpdate();
      swiWaitForVBlank();
   }
}

int KanjiQuizMode::PrintScreens (const Card& card)
{
  int selected_card[4] = {0,0,0,0};
  selected_card[0] = card.index();
  // Fills the vector with not repeated random cards
  for (size_t i = 1; i < 4; ++i)
  {
    bool chosen;
    do
    {
      chosen = false;
      selected_card[i] = rand()%xmlparser_->package_records() + 1;
      for (size_t j = 0;!chosen && j < 4; ++j)
        chosen = (i!=j) && (selected_card[j] == selected_card[i]);
    }while(chosen);
  }

  // randomizes the correct card position
  int new_position = rand()%4;
  int aux = selected_card[new_position];
  selected_card[new_position] = selected_card[0];
  selected_card[0] = aux;

  translation_->text(card.translation());
	on_reading_->text(card.reading());
	kun_reading_->text(card.reading2());
#if SUB
	kanji1_->text(xmlparser_->card(selected_card[0]).symbol());
	kanji2_->text(xmlparser_->card(selected_card[1]).symbol());
	kanji3_->text(xmlparser_->card(selected_card[2]).symbol());
	kanji4_->text(xmlparser_->card(selected_card[3]).symbol());
#endif
	tbh_->PrintAll();

	return new_position + 1;
}

void KanjiQuizMode::PrintBitmap (int x, int y, int bgid,
                                 const unsigned int* bitmap, int palette_offset)
{
  // FIXME: Don't print correctly
  u16* video_buffer = bgGetGfxPtr(bgid);
  for (int image_y = 0; image_y < 20; ++image_y)
    for (int image_x = 0; image_x < 20; ++image_x)
    {
      int pixel_num = image_y * 20 + image_x;
      // select the indicated byte in the word and adds the offset
      // for the new palette
      int pixel_palette_index =
        ((bitmap [(pixel_num >> 2)] & (0xFF000000 >> (pixel_num % 4))
        ) >> ((3-pixel_num%4) << 3 )) + palette_offset;
      int video_index = (image_y + y)*128 + (image_x + x)/2;
      if ((image_x+x) % 2)
      {
        // odd
        video_buffer[video_index] = pixel_palette_index |
                                    (video_buffer[video_index] & 0xFF00);
      }
      else
      {
        // even
        video_buffer[video_index] = (pixel_palette_index << 8) |
                                    (video_buffer[video_index] & 0x00FF);
      }
    }
}

KanjiQuizMode::~KanjiQuizMode ()
{
  // it destroys all its text boxes
  tbh_->~TextBoxHandler();
  delete tbh_;
  xmlparser_->~XMLParser();
  delete xmlparser_;
}


