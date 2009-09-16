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
#include <maxmod9.h>

#include "efs_lib.h"		// include EFS lib

#include <string>
#include <vector>
#include "xmlparser.h"
#include "screenshandler.h"
#include "mainscreenhandler.h"
#include "subscreenhandler.h"
#include "soundhandler.h"
#include "dsmemorizer.h"

int main (void)
{
	fatInitDefault();
  if (EFS_Init (EFS_AND_FAT | EFS_DEFAULT_DEVICE, NULL))
  {
    DSMemorizer dsmemorizer;
    dsmemorizer.Init();
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

