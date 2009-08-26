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
#include "kanjimode.h"
#include "card.h"

using namespace std;
int main (void)
{
  irqInit();
  irqEnable(IRQ_VBLANK);
  consoleDemoInit ();

  if (EFS_Init (EFS_AND_FAT | EFS_DEFAULT_DEVICE, NULL))
  {
    //iprintf ("EFS init OK!\n");
    //iprintf ("found NDS path: %s\n", efs_path);

    //set video mode to mode 5
    videoSetMode (MODE_5_2D);

    //map vram a to start of main background graphics memory
    vramSetBankA (VRAM_A_MAIN_BG_0x06000000);

    int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

    KanjiMode km;
    km.Init(bg3);
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

