/// \file soundhandler.h Sound Handling.
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

#ifndef SOUNDHANDLER_H_
#define SOUNDHANDLER_H_

/// Game sounds handler
class SoundHandler
{
public:
  /// Sound effects available
  enum SFX
  {
    ACTION,       ///< Sound played when doing an action
    THEME         ///< Main theme for the main menu.
  };

  SoundHandler () {}
  void Init ();
  void PlayEffect (SFX sfx);
  void StopEffect (SFX sfx);
  void UnloadEffect (SFX sfx);
  void UnloadEffects ();

  ~SoundHandler();
private:
  mm_sfxhand* sfx_handlers_;
  bool* loaded_sfx_;
  mm_sound_effect* sfx_available_;
};



#endif // SOUNDHANDLER_H_
