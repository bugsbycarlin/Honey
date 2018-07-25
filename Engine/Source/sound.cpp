/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "sound.h"

namespace Honey {
  Sound* sound = new Sound();

  Sound::Sound() {
    sounds = {};
    music = {};
  }

  void Sound::initialize() {
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
  }

  void Sound::setSoundVolume(float fraction) {
    Mix_Volume(-1, (int) (MIX_MAX_VOLUME * fraction));
  }

  void Sound::setMusicVolume(float fraction) {
    Mix_VolumeMusic((int) (MIX_MAX_VOLUME * fraction));
  }  

  void Sound::addSound(std::string label, std::string path) {
    Mix_Chunk *sound;
    sound = Mix_LoadWAV(path.c_str());
    sounds[label] = sound;
  }

  void Sound::playSound(std::string label, int loops) {
    if (sounds.count(label) == 0) {
      printf("Failed to find %s in sounds.\n", label.c_str());
      return;
    }

    if (loops > 0) {
      // In PlayChannel, 0 is 1 loop, 1 is two loops, etc.
      loops--;
    }

    Mix_PlayChannel(-1, sounds[label], loops);
  }

  void Sound::destroySound(std::string label) {
    if (sounds.count(label) == 0) {
      printf("Failed to delete %s because it wasn't in sounds.\n", label.c_str());
      return;
    }

    Mix_FreeChunk(sounds[label]);
    sounds.erase(label);
  }

  void Sound::destroyAllSounds() {
    for (std::pair<std::string, Mix_Chunk*> item : sounds) {
      Mix_FreeChunk(item.second);
      sounds.erase(item.first);
    }
  }

  void Sound::addMusic(std::string label, std::string path) {
    Mix_Music *musica;
    musica = Mix_LoadMUS(path.c_str());
    music[label] = musica;
  }

  void Sound::playMusic(std::string label, int loops) {
    if (music.count(label) == 0) {
      printf("Failed to find %s in music.\n", label.c_str());
      return;
    }

    Mix_PlayMusic(music[label], loops);
  }

  void Sound::destroyMusic(std::string label) {
    if (music.count(label) == 0) {
      printf("Failed to delete %s because it wasn't in sounds.\n", label.c_str());
      return;
    }

    Mix_FreeMusic(music[label]);
    music.erase(label);
  }

  // NOOOOOOOOOOOOOOOOO
  void Sound::destroyAllMusic() {
    for (std::pair<std::string, Mix_Music*> item : music) {
      Mix_FreeMusic(item.second);
      music.erase(item.first);
    }
  }
}