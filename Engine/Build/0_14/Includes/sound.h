/*

  Honey
  Copyright 2018 - Matthew Carlin

  Sound class wraps SDL sound functions.
*/

#pragma once

#include <string>
#include <stdlib.h>
#include <unordered_map>

#include <SDL2/SDL.h>
#include "SDL2_mixer/SDL_mixer.h"

using namespace std;

namespace Honey {
  class Sound {
   public:
      Sound();

      void initialize();

      // From 0 silent to 1 full volume
      void setSoundVolume(float fraction);
      void setMusicVolume(float fraction);

      void addSound(string label, string path);
      //-1 loops forever
      void playSound(string label, int loops);
      void destroySound(string label);
      void destroyAllSounds();

      void addMusic(string label, string path);
      //-1 loops forever
      void playMusic(string label, int loops);
      void destroyMusic(string label);
      void destroyAllMusic();

      ~Sound();

    private:
      unordered_map<string, Mix_Chunk*> sounds;
      unordered_map<string, Mix_Music*> music;
  };

  extern Sound* sound;
}
