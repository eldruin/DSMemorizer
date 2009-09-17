/// \file definitions.h Definitions of positions and static constant numeric stuff.
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

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include "types.h"

static const Types::Font CAPTION_FONT = Types::VERA_FONT;
static const Types::Font NORMAL_FONT = Types::MONA_FONT;

// Kanji mode definitions
static const int KM_CAPTION_SIZE = 8;
static const int KM_NORMAL_SIZE = 10;
static const int KM_KANJI_SIZE = 30;
static const int KM_CAPTION_KANJI_X = 15;
static const int KM_CAPTION_KANJI_Y = 5;
static const int KM_KANJI_X = 12;
static const int KM_KANJI_Y = 23;
static const int KM_CAPTION_ON_READING_X = 70;
static const int KM_CAPTION_ON_READING_Y = 5;
static const int KM_ON_READING_X = 70;
static const int KM_ON_READING_Y = 23;
static const int KM_CAPTION_KUN_READING_X = 70;
static const int KM_CAPTION_KUN_READING_Y = 43;
static const int KM_KUN_READING_X = 70;
static const int KM_KUN_READING_Y = 60;
static const int KM_CAPTION_TRANSLATION_X = 10;
static const int KM_CAPTION_TRANSLATION_Y = 80;
static const int KM_TRANSLATION_X = 10;
static const int KM_TRANSLATION_Y = 97;
static const int KM_CAPTION_EXAMPLE_X = 10;
static const int KM_CAPTION_EXAMPLE_Y = 117;
static const int KM_EXAMPLE_KANJI_X = 10;
static const int KM_EXAMPLE_KANJI_Y = 134;
static const int KM_EXAMPLE_READING_X = 10;
static const int KM_EXAMPLE_READING_Y = 154;
static const int KM_EXAMPLE_TRANSLATION_X = 10;
static const int KM_EXAMPLE_TRANSLATION_Y = 174;

// Vertical TextBoxes Definitions
static const int VTB_CAPTION_SIZE = 8;
static const int VTB_NORMAL_SIZE = 10;
static const int VTB_CAPTION_BOX1_X = 20;
static const int VTB_CAPTION_BOX1_Y = 18;
static const int VTB_BOX1_X = 20;
static const int VTB_BOX1_Y = 33;
static const int VTB_CAPTION_BOX2_X = 20;
static const int VTB_CAPTION_BOX2_Y = 53;
static const int VTB_BOX2_X = 20;
static const int VTB_BOX2_Y = 70;
static const int VTB_CAPTION_BOX3_X = 20;
static const int VTB_CAPTION_BOX3_Y = 90;
static const int VTB_BOX3_X = 20;
static const int VTB_BOX3_Y = 107;

// Cards subscreen mode definitions
static const int CARDS_TEXT_SIZE = 8;
static const int CARDS_TEXT_X = 70; 
static const int CARDS_TEXT_Y = 8;

// Kanji choose subscreen mode definitions
static const int KC_CAPTION_SIZE = 8;
static const int KC_NORMAL_SIZE = 30;
static const int KC_SCOREBOARD_X = 154;
static const int KC_SCOREBOARD_Y = 13;
static const int KC_ACCURACY_X = 23;
static const int KC_ACCURACY_Y = 13;
static const int KC_BOX1_X = 33;
static const int KC_BOX1_Y = 35;
static const int KC_BOX2_X = 85;
static const int KC_BOX2_Y = 35;
static const int KC_BOX3_X = 137;
static const int KC_BOX3_Y = 35;
static const int KC_BOX4_X = 189;
static const int KC_BOX4_Y = 35;

// Vertical TextBoxes choose subscreen mode definitions
static const int VTBC_CAPTION_SIZE = 8;
static const int VTBC_NORMAL_SIZE = 10;
static const int VTBC_SCOREBOARD_X = 154;
static const int VTBC_SCOREBOARD_Y = 13;
static const int VTBC_ACCURACY_X = 23;
static const int VTBC_ACCURACY_Y = 13;
static const int VTBC_BOX1_X = 33;
static const int VTBC_BOX1_Y = 39;
static const int VTBC_BOX2_X = 33;
static const int VTBC_BOX2_Y = 68;
static const int VTBC_BOX3_X = 33;
static const int VTBC_BOX3_Y = 97;
static const int VTBC_BOX4_X = 33;
static const int VTBC_BOX4_Y = 129;

// Tick and cross positions
static const int TICK_CROSS_X_1 = 43;
static const int TICK_CROSS_X_2 = 98;
static const int TICK_CROSS_X_3 = 153;
static const int TICK_CROSS_X_4 = 200;
static const int TICK_CROSS_Y_1 = 39;
static const int TICK_CROSS_Y_2 = 68;
static const int TICK_CROSS_Y_3 = 97;
static const int TICK_CROSS_Y_4 = 126;
static const int KC_TICK_CROSS_X = 10;
static const int KC_TICK_CROSS_Y = 103;

// Grade and strokes options
static const int OGS_TITLE_FONT_SIZE = 14;
static const int OGS_TITLE_X = 48;
static const int OGS_TITLE_Y = 12;
static const int OGS_CAPTION_FONT_SIZE = 10;
static const int OGS_BOX_FONT_SIZE = 10;
static const int OGS_CAPTION_GRADE_X = 30;
static const int OGS_CAPTION_GRADE_Y = 67;
static const int OGS_CAPTION_STROKES_X = 25;
static const int OGS_CAPTION_STROKES_Y = 97;
static const int OGS_GRADE_MIN_X = 113;
static const int OGS_GRADE_MIN_Y = 68;
static const int OGS_GRADE_MAX_X = 190;
static const int OGS_GRADE_MAX_Y = 68;
static const int OGS_STROKES_MIN_X = 107;
static const int OGS_STROKES_MIN_Y = 97;
static const int OGS_STROKES_MAX_X = 185;
static const int OGS_STROKES_MAX_Y = 97;

static const int TP_LEFT_BOTTOM_IMAGE_X1 = 5;
static const int TP_LEFT_BOTTOM_IMAGE_X2 = 39;
static const int TP_LEFT_BOTTOM_IMAGE_Y1 = 152;
static const int TP_LEFT_BOTTOM_IMAGE_Y2 = 187;

static const int TP_RIGHT_BOTTOM_IMAGE_X1 = 210;
static const int TP_RIGHT_BOTTOM_IMAGE_X2 = 250;
static const int TP_RIGHT_BOTTOM_IMAGE_Y1 = 146;
static const int TP_RIGHT_BOTTOM_IMAGE_Y2 = 187;

static const int TP_CARD1_X1 = 36;
static const int TP_CARD1_X2 = 102;
static const int TP_CARD2_X1 = 156;
static const int TP_CARD2_X2 = 224;
static const int TP_CARD_Y1 = 40;
static const int TP_CARD_Y2 = 146;
static const int TP_CARD_MIDDLE_Y = 90;

static const int TP_OGS_MINUS1_X1 = 84;
static const int TP_OGS_MINUS1_X2 = 101;
static const int TP_OGS_MINUS2_X1 = 160;
static const int TP_OGS_MINUS2_X2 = 180;
static const int TP_OGS_PLUS1_X1 = 132;
static const int TP_OGS_PLUS1_X2 = 154;
static const int TP_OGS_PLUS2_X1 = 208;
static const int TP_OGS_PLUS2_X2 = 230;
static const int TP_OGS_GRADE_Y1 = 70;
static const int TP_OGS_GRADE_Y2 = 87;
static const int TP_OGS_STROKES_Y1 = 96;
static const int TP_OGS_STROKES_Y2 = 115;

static const int MAX_CARD_NUMBER_TEXT_LENGTH = 40;
static const int MAX_SCORE_TEXT_LENGTH = 40;
static const int MAX_ACCURACY_TEXT_LENGTH = 40;
static const unsigned MIN_GRADE = 1;
static const unsigned MAX_GRADE = 9;
static const unsigned MIN_STROKES = 1;
static const unsigned MAX_STROKES = 25;


#endif // DEFINITIONS_H_

