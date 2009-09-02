/// \file vocabularyquizmode.h Vocabulary quiz game mode.
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

#ifndef VOCABULARYQUIZMODE_H_
#define VOCABULARYQUIZMODE_H_

namespace GameMode
{
  /// Vocabulary quiz game mode.
  /// Shows a word and its reading and asks for the kanji version.
  void VocabularyQuizMode (MainScreenHandler* main_screen_handler,
                           SubScreenHandler* sub_screen_handler);
}

#endif // VOCABULARYQUIZMODE_H_

