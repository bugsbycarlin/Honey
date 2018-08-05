/*!
  @Honey
  @author Matthew Carlin
  Copyright 2018
*/

#pragma once

#include <string>
#include <stdlib.h>
#include <unordered_map>

#include <SDL2/SDL.h>
#include "SDL2_mixer/SDL_mixer.h"

using namespace std;

namespace Honey {
  /*!
    The Sound class wraps SDL sound functions.

    We guarantee that the sound loader is compatible with wav files, and the music loader is compatible with mp3 files.

    SDL mixer may add additional functionality by default, but we don't support it.

    Primarily covered in blog posts:

    http://www.friendsonmountains.com/blog/2018/07/17/lets-make-honey-version-0-09-sounds-good
  */
  class Sound final {
   public:
    /*!
      Singleton instance getter.

      @return This'll get you the one instance of Sound you're supposed to have.
    */
    static Sound& instance();

    /*!
      Initialize Sound by performing basic SDL mixer setup.
    */
    void initialize();

    /*!
      Set Sound Volume.

      @param fraction float between 0 (silent) and 1 (full volume).
    */
    void setSoundVolume(float fraction);

    /*!
      Set Music Volume.

      @param fraction float between 0 (silent) and 1 (full volume).
    */
    void setMusicVolume(float fraction);

    /*!
      Add a sound to this system.

      @param label The sound will be stored by this name (eg "cheer").
      @param path OS valid path of a wav file (eg "Sound/cheer.wav").
    */
    void addSound(string label, string path);

    /*!
      Play a sound a certain number of times.

      @param label The sound.
      @param loops The number of times to loop. -1 loops the sound forever.
    */
    void playSound(string label, int loops);

    /*!
      Remove a sound from this system.

      @param label The sound.
    */
    void removeSound(string label);

    /*!
      Remove all sounds from this system.
    */
    void removeAllSounds();

    /*!
      Add music to this system.

      @param label The music will be stored by this name (eg "bearJamboreeDubstep").
      @param path OS valid path of an mp3 file (eg "Sound/bearJamboreeDubstep.mp3").
    */
    void addMusic(string label, string path);
    
    /*!
      Play music a certain number of times.

      @param label The music.
      @param loops The number of times to loop. -1 loops the music forever.
    */
    void playMusic(string label, int loops);

    /*!
      Remove music from this system.

      @param label The music.
    */
    void removeMusic(string label);

    /*!
      Remove all music from this system.
    */
    void removeAllMusic();

   private:
    Sound();
    ~Sound();

    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;
    Sound(Sound&&) = delete;
    Sound& operator=(Sound&&) = delete;

    unordered_map<string, Mix_Chunk*> sounds;
    unordered_map<string, Mix_Music*> music;
  };

  extern Sound& sound;
}
