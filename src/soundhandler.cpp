/// \file soundhandler.cpp Sound Handling.
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
#include <maxmod9.h>
#include <stdio.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#include "soundhandler.h"

void SoundHandler::Init()
{
  mmInitDefaultMem((mm_addr)soundbank_bin);

  sfx_handlers_ = new mm_sfxhand [MSL_NSAMPS];
  loaded_sfx_ = new bool [MSL_NSAMPS];
  sfx_available_ = new mm_sound_effect [MSL_NSAMPS];

  for (size_t i = 0; i < MSL_NSAMPS; ++i)
    loaded_sfx_[i] = false;

  sfx_available_[ACTION].id = SFX_DEERCHASER;
	sfx_available_[ACTION].rate = 1024;
  sfx_available_[ACTION].handle = 0;
  sfx_available_[ACTION].volume = 60;
  sfx_available_[ACTION].panning = 150;

	sfx_available_[THEME].id = SFX_FLUTE;
  sfx_available_[THEME].rate = 1024;
  sfx_available_[THEME].handle = 0;
  sfx_available_[THEME].volume = 255;
  sfx_available_[THEME].panning = 128;
}

void SoundHandler::PlayEffect(SoundHandler::SFX sfx)
{
  switch (sfx)
  {
    case ACTION:
      if (!loaded_sfx_[ACTION])
      {
        mmLoadEffect(SFX_DEERCHASER);
        loaded_sfx_[ACTION] = true;
      }
      sfx_handlers_[ACTION] = mmEffectEx (&(sfx_available_[ACTION]));
      break;
    case THEME:
      if (!loaded_sfx_[THEME])
      {
        mmLoadEffect(SFX_FLUTE);
        loaded_sfx_[THEME] = true;
      }
      sfx_handlers_[THEME] = mmEffectEx (&(sfx_available_[THEME]));
      break;
  }
}

void SoundHandler::StopEffect(SoundHandler::SFX sfx)
{
  switch (sfx)
  {
    case ACTION:
      if (loaded_sfx_[ACTION])
        mmEffectCancel(sfx_handlers_[ACTION]);
      break;
    case THEME:
      if (loaded_sfx_[THEME])
        mmEffectCancel(sfx_handlers_[THEME]);
      break;
  }
}

void SoundHandler::UnloadEffect (SFX sfx)
{
  mmEffectCancel(sfx_handlers_[sfx]);
  mmUnloadEffect(sfx_available_[sfx].id);
  loaded_sfx_[sfx] = false;
}

void SoundHandler::UnloadEffects ()
{
  for (size_t i = 0; i < MSL_NSAMPS; ++i)
  {
    mmEffectCancel(sfx_handlers_[i]);
    mmUnloadEffect(sfx_available_[i].id);
    loaded_sfx_[i] = false;
  }
}

SoundHandler::~SoundHandler ()
{
  UnloadEffects();
  delete sfx_available_;
  delete sfx_handlers_;
  delete loaded_sfx_;
}
