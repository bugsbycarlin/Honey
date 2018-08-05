/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "sound.h"

using namespace std;

namespace Honey {
  Sound& Sound::instance() {
    static Sound sound_instance;
    return sound_instance;
  }

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

  void Sound::addSound(string label, string path) {
    Mix_Chunk *sound;
    sound = Mix_LoadWAV(path.c_str());
    sounds[label] = sound;
  }

  void Sound::playSound(string label, int loops) {
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

  void Sound::removeSound(string label) {
    if (sounds.count(label) == 0) {
      printf("Failed to delete %s because it wasn't in sounds.\n", label.c_str());
      return;
    }

    Mix_FreeChunk(sounds[label]);
    sounds.erase(label);
  }

  void Sound::removeAllSounds() {
    for (pair<string, Mix_Chunk*> item : sounds) {
      Mix_FreeChunk(item.second);
    }
    sounds.clear();
  }

  void Sound::addMusic(string label, string path) {
    Mix_Music *musica;
    musica = Mix_LoadMUS(path.c_str());
    music[label] = musica;
  }

  void Sound::playMusic(string label, int loops) {
    if (music.count(label) == 0) {
      printf("Failed to find %s in music.\n", label.c_str());
      return;
    }

    Mix_PlayMusic(music[label], loops);
  }

  void Sound::removeMusic(string label) {
    if (music.count(label) == 0) {
      printf("Failed to delete %s because it wasn't in sounds.\n", label.c_str());
      return;
    }

    Mix_FreeMusic(music[label]);
    music.erase(label);
  }

  // NOOOOOOOOOOOOOOOOO
  void Sound::removeAllMusic() {
    for (pair<string, Mix_Music*> item : music) {
      Mix_FreeMusic(item.second);
    }
    music.clear();
  }

  Sound::~Sound() {
    removeAllMusic();
    removeAllSounds();
  }
}
