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
#include "kanjimodescreen.h"

KanjiModeScreen::KanjiModeScreen(int bgid)
{
	video_buffer_ = bgGetGfxPtr(bgid);
	kanji_.Init(bgid);
	kanji_.LoadFace(Types::MONA_FONT, 30);
	first_reading_.Init(bgid);
	first_reading_.LoadFace(Types::MONA_FONT, 16);
	second_reading_.Init(bgid);
	second_reading_.LoadFace(Types::MONA_FONT, 16);
	translation_.Init(bgid);
	translation_.LoadFace(Types::MONA_FONT, 16);
	example_kanji_.Init(bgid);
	example_kanji_.LoadFace(Types::MONA_FONT, 16);
	example_reading_.Init(bgid);
	example_reading_.LoadFace(Types::MONA_FONT, 16);
	example_translation_.Init(bgid);
	example_translation_.LoadFace(Types::MONA_FONT, 16);
}

void KanjiModeScreen::kanji (const std::string& str)
{
  int x = 10, y = 50;
	kanji_.Print(str, x, y);
}

void KanjiModeScreen::first_reading (const std::string& str)
{
	int x = 50, y = 30;
	first_reading_.Print(str, x, y);
}

void KanjiModeScreen::second_reading (const std::string& str)
{
	int x = 50, y = 60;
	second_reading_.Print(str, x, y);
}

void KanjiModeScreen::translation (const std::string& str)
{
	int x = 10, y = 90;
	translation_.Print(str, x, y);
}

void KanjiModeScreen::example_kanji (const std::string& str)
{
	int x = 10, y = 120;
	example_kanji_.Print(str, x, y);
}

void KanjiModeScreen::example_reading (const std::string& str)
{
	int x = 10, y = 150;
	example_reading_.Print(str, x, y);
}

void KanjiModeScreen::example_translation (const std::string& str)
{
	int x = 10, y = 180;
	example_translation_.Print(str, x, y);
}


