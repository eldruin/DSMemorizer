/// \file main.cpp Main file.
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
#include <stdio.h>
#include <malloc.h>
#include <fat.h>
#include <unistd.h>		// filesystem functions

#include "efs_lib.h"		// include EFS lib
#include "utf8.h"		// UTF-8 string library

#include "types.h"
#include "text.h"
#include "kanjimodescreen.h"

using namespace std;
int main (void)
{
  irqInit();
  irqEnable(IRQ_VBLANK);
  int position = 0;
  consoleDemoInit ();
  iprintf ("hello\n");

  if (EFS_Init (EFS_AND_FAT | EFS_DEFAULT_DEVICE, NULL))
  {
    iprintf ("EFS init OK!\n");
    iprintf ("found NDS path: %s\n", efs_path);

    int error;

    //point our video buffer to the start of bitmap background video
    u16 *video_buffer = (u16 *) BG_BMP_RAM (0);

    //set video mode to mode 5
    videoSetMode (MODE_5_2D);

    //map vram a to start of main background graphics memory
    vramSetBankA (VRAM_A_MAIN_BG_0x06000000);

    int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

    BG_PALETTE[0]= RGB15(31,31,31);
    BG_PALETTE[Types::Color::BLACK]= RGB15(0,0,0);
    BG_PALETTE[Types::Color::GREY]= RGB15(15,15,15);
    BG_PALETTE[Types::Color::WHITE]= RGB15(31,31,31);
    BG_PALETTE[255]= RGB15(0,0,0);
    
    KanjiModeScreen kms(bg3);
    kms.kanji("日");
    kms.first_reading("び、か");
    kms.second_reading("ひ");
    kms.translation("Day, sun");
    kms.example_kanji("今日");
    kms.example_reading("きょう");
    kms.example_translation ("Today");
  }
  else
  {
    iprintf ("EFS init error!\n");
  }
  while (1)
  {
    swiWaitForVBlank ();
  }

  return 0;
}

