/// \file kanjimodescreen.cpp Kanji mode screen manager.
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

#include "nds.h"
#include "types.h"
#include "text.h"
#include "card.h"
#include "kanjimodescreen.h"

void KanjiModeScreen::Init(int bgid)
{
	video_buffer_ = bgGetGfxPtr(bgid);

	// Init all the text
	kanji_.Init(bgid);
	kanji_.LoadFace(Types::MONA_FONT, 30);
	first_reading_.Init(bgid);
	first_reading_.LoadFace(Types::MONA_FONT, 10);
	second_reading_.Init(bgid);
	second_reading_.LoadFace(Types::MONA_FONT, 10);
	translation_.Init(bgid);
	translation_.LoadFace(Types::MONA_FONT, 10);
	example_kanji_.Init(bgid);
	example_kanji_.LoadFace(Types::MONA_FONT, 10);
	example_reading_.Init(bgid);
	example_reading_.LoadFace(Types::MONA_FONT, 10);
	example_translation_.Init(bgid);
	example_translation_.LoadFace(Types::MONA_FONT, 10);
}

void KanjiModeScreen::Print (const Card& card)
{
  int x = 10, y = 50;
	kanji_.Print(card.symbol(), x, y);
	x = 50, y = 30;
	first_reading_.Print(card.reading(), x, y);
	x = 50, y = 60;
	second_reading_.Print(card.reading2(), x, y);
	x = 10, y = 90;
	translation_.Print(card.translation(), x, y);
	x = 10, y = 120;
	example_kanji_.Print(card.example_symbol(), x, y);
	x = 10, y = 150;
	example_reading_.Print(card.example_reading(), x, y);
	x = 10, y = 180;
	example_translation_.Print(card.example_translation(), x, y);
}
