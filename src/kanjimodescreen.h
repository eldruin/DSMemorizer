/// \file kanjimodescreen.h Kanji mode screen manager.
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

#ifndef KANJIMODESCREEN_H_
#define KANJIMODESCREEN_H_

class Text;

class KanjiModeScreen
{
public:
	/// Constructor
	KanjiModeScreen(int bgid);
	void kanji (const std::string& str);
	void first_reading (const std::string& str);
	void second_reading (const std::string& str);
	void translation (const std::string& str);
	void example_kanji (const std::string& str);
	void example_reading (const std::string& str);
	void example_translation (const std::string& str);

private:
	Text kanji_, first_reading_, second_reading_, translation_, 
		example_kanji_, example_reading_, example_translation_;
		
	u16* video_buffer_;
};

#endif // KANJIMODESCREEN_H_

