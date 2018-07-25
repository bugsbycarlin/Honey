/*

  Honey
  Copyright 2018 - Matthew Carlin

  Sound class wraps SDL sound functions.
*/

#ifndef HONEY_SOUND_H_
#define HONEY_SOUND_H_

#include <string>
#include <stdlib.h>
#include <unordered_map>

#include <SDL2/SDL.h>
#include "SDL2_mixer/SDL_mixer.h"

namespace Honey {
  class Sound {
   public:

      std::unordered_map<std::string, Mix_Chunk*> sounds;
      std::unordered_map<std::string, Mix_Music*> music;

      Sound();

      void initialize();

      // From 0 silent to 1 full volume
      void setSoundVolume(float fraction);
      void setMusicVolume(float fraction);

      void addSound(std::string label, std::string path);
      //-1 loops forever
      void playSound(std::string label, int loops);
      void destroySound(std::string label);
      void destroyAllSounds();

      void addMusic(std::string label, std::string path);
      //-1 loops forever
      void playMusic(std::string label, int loops);
      void destroyMusic(std::string label);
      void destroyAllMusic();
  };

  extern Sound* sound;
}
#endif
