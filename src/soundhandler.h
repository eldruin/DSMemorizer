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

  /// Default constructor
  SoundHandler () {}
  /// Initializer
  void Init ();
  /// Load an effect in memory if it's not loaded and then play it.
  void PlayEffect (SFX sfx);
  /// Stop playing an effect.
  void StopEffect (SFX sfx);
  /// Unload an specific effect from memory. Will stop playing before unload in
  /// case it were being played.
  void UnloadEffect (SFX sfx);
  /// Unload all the effects. Will stop playing before unload in case any were
  /// being played.
  void UnloadEffects ();

  /// Destructor
  ~SoundHandler();
private:
  /// SFX handler vector
  mm_sfxhand* sfx_handlers_;
  /// SFX loaded or not vector.
  bool* loaded_sfx_;
  /// Structures for all the sound effects available in the sound bank.
  mm_sound_effect* sfx_available_;
};



#endif // SOUNDHANDLER_H_
