/*

  Honey
  Copyright 2018 - Matthew Carlin

  Input class wraps SDL input methods to create an event handling system.
*/

#ifndef HONEY_INPUT_H_
#define HONEY_INPUT_H_

#include <string>
#include <stdlib.h>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "logic.h"

namespace Honey {
  class Input {
   public:
      std::unordered_map<std::string, std::string> action_to_key;

      std::unordered_map<std::string, bool> down;
      std::unordered_map<std::string, bool> up;
      std::unordered_map<std::string, int> pressed;

      Input();

      void processInput();
      std::string translateToKey(SDL_Event event);

      int keyPressed(std::string key);
      bool keyDown(std::string key);
      bool keyUp(std::string key);
      bool threeQuickKey(std::string key);

      int actionPressed(std::string action);
      bool actionDown(std::string action);
      bool actionUp(std::string action);
      bool threeQuickAction(std::string action);

      void addActionKey(std::string action, std::string key);
      void deleteAction(std::string action);
  };

  extern Input* input;
}
#endif
