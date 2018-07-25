/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "input.h"

namespace Honey {
  Input* input = new Input();

  Input::Input() {
    action_to_key = {};
  }

  void Input::processInput() {
    // Clear the list of things that were pressed and the list of things that were up,
    // but preserve the list of things that are down.
    pressed = {};
    up = {};

    // Handle the events in the queue
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
      
      if (event.type == SDL_QUIT) {
        // Special input: quitting! The user can for instance ask
        // to quit by pressing a window close button.
        pressed["quit"] = 1;

      } else if (event.type == SDL_KEYDOWN) {
        // If we see a key down, translate it using the helper method,
        // clear it from the up map, and mark it in the down map.
        // Also, if it's not a repeat, mark it in the pressed map.
        std::string key = translateToKey(event);
        up.erase(key);
        down[key] = true;
        if (!event.key.repeat) {
          if (pressed.count(key) == 0) {
            pressed[key] = 0;
          }
          pressed[key] += 1;
        }

      } else if (event.type == SDL_KEYUP) {
        // If we see a key up, translate it using the helper method,
        // clear it from the down map, and mark it in the up map.
        std::string key = translateToKey(event);
        down.erase(key);
        up[key] = true;
      }
    }
  }

  std::string Input::translateToKey(SDL_Event event) {
    std::string key = "unknown";
    switch (event.key.keysym.sym) {
      case SDLK_UP : key = "up"; break;
      case SDLK_DOWN : key = "down"; break;
      case SDLK_RIGHT : key = "right"; break;
      case SDLK_LEFT : key = "left"; break;
      case SDLK_BACKSPACE : key = "backspace"; break;
      case SDLK_TAB : key = "tab"; break;
      case SDLK_SPACE : key = "space"; break;
      case SDLK_ESCAPE : key = "escape"; break;
      case SDLK_RETURN : key = "return"; break;
      case SDLK_DELETE : key = "delete"; break;
      case SDLK_BACKSLASH : key = "backslash"; break;
      case SDLK_QUOTE : key = "quote"; break;
      case SDLK_QUOTEDBL : key = "doublequote"; break;
      case SDLK_PLUS : key = "+"; break;
      case SDLK_COMMA : key = ","; break;
      case SDLK_MINUS : key = "-"; break;
      case SDLK_PERIOD : key = "."; break;
      case SDLK_SLASH : key = "/"; break;
      case SDLK_COLON : key = ":"; break;
      case SDLK_SEMICOLON : key = ";"; break;
      case SDLK_LESS : key = "<"; break;
      case SDLK_EQUALS : key = "="; break;
      case SDLK_GREATER : key = ">"; break;
      case SDLK_QUESTION : key = "?"; break;
      case SDLK_AT : key = "@"; break;
      case SDLK_LEFTBRACKET : key = "["; break;
      case SDLK_RIGHTBRACKET : key = "]"; break;
      case SDLK_CARET : key = "^"; break;
      case SDLK_UNDERSCORE : key = "_"; break;
      case SDLK_EXCLAIM : key = "!"; break;
      case SDLK_HASH : key = "#"; break;
      case SDLK_DOLLAR : key = "$"; break;
      case SDLK_PERCENT : key = "%"; break;
      case SDLK_AMPERSAND : key = "&"; break;
      case SDLK_LEFTPAREN : key = "("; break;
      case SDLK_RIGHTPAREN : key = ")"; break;
      case SDLK_ASTERISK : key = "*"; break;
      case SDLK_0 : key = "0"; break;
      case SDLK_1 : key = "1"; break;
      case SDLK_2 : key = "2"; break;
      case SDLK_3 : key = "3"; break;
      case SDLK_4 : key = "4"; break;
      case SDLK_5 : key = "5"; break;
      case SDLK_6 : key = "6"; break;
      case SDLK_7 : key = "7"; break;
      case SDLK_8 : key = "8"; break;
      case SDLK_9 : key = "9"; break;
      case SDLK_KP_0 : key = "0"; break;
      case SDLK_KP_1 : key = "1"; break;
      case SDLK_KP_2 : key = "2"; break;
      case SDLK_KP_3 : key = "3"; break;
      case SDLK_KP_4 : key = "4"; break;
      case SDLK_KP_5 : key = "5"; break;
      case SDLK_KP_6 : key = "6"; break;
      case SDLK_KP_7 : key = "7"; break;
      case SDLK_KP_8 : key = "8"; break;
      case SDLK_KP_9 : key = "9"; break;
      case SDLK_a : key = "a"; break;
      case SDLK_b : key = "b"; break;
      case SDLK_c : key = "c"; break;
      case SDLK_d : key = "d"; break;
      case SDLK_e : key = "e"; break;
      case SDLK_f : key = "f"; break;
      case SDLK_g : key = "g"; break;
      case SDLK_h : key = "h"; break;
      case SDLK_i : key = "i"; break;
      case SDLK_j : key = "j"; break;
      case SDLK_k : key = "k"; break;
      case SDLK_l : key = "l"; break;
      case SDLK_m : key = "m"; break;
      case SDLK_n : key = "n"; break;
      case SDLK_o : key = "o"; break;
      case SDLK_p : key = "p"; break;
      case SDLK_q : key = "q"; break;
      case SDLK_r : key = "r"; break;
      case SDLK_s : key = "s"; break;
      case SDLK_t : key = "t"; break;
      case SDLK_u : key = "u"; break;
      case SDLK_v : key = "v"; break;
      case SDLK_w : key = "w"; break;
      case SDLK_x : key = "x"; break;
      case SDLK_y : key = "y"; break;
      case SDLK_z : key = "z"; break;
    }
    return key;
  }

  int Input::keyPressed(std::string key) {
    if (pressed.count(key) == 1) {
      return pressed[key];
    }
    return 0;
  }

  bool Input::keyDown(std::string key) {
    if (down.count(key) == 1) {
      return true;
    }
    return false;
  }

  bool Input::keyUp(std::string key) {
    if (up.count(key) == 1) {
      return true;
    }
    return false;
  }

  bool Input::threeQuickKey(std::string key) {
    if (keyPressed(key) > 0) {
      std::string label = key + "_quick_counter";
      if (logic->transientCounterValue(label) <= 0) {
        logic->makeTransientCounter(label, 1.0);
      }

      logic->incrementTransientCounter(label, keyPressed(key));

      if (logic->transientCounterValue(label) >= 3) {
        return true;
      }
    }
    return false;
  }

  int Input::actionPressed(std::string action) {
    if (action_to_key.count(action) == 0) {
      return 0;
    } else {
      return keyPressed(action_to_key[action]);
    }
  }

  bool Input::actionDown(std::string action) {
    if (action_to_key.count(action) == 0) {
      return false;
    } else {
      return keyDown(action_to_key[action]);
    }
  }

  bool Input::actionUp(std::string action) {
    if (action_to_key.count(action) == 0) {
      return false;
    } else {
      return keyUp(action_to_key[action]);
    }
  }

  bool Input::threeQuickAction(std::string action) {
    if (actionPressed(action) > 0) {
      std::string label = action + "_quick_counter";
      if (logic->transientCounterValue(label) <= 0) {
        logic->makeTransientCounter(label, 1.0);
      }

      logic->incrementTransientCounter(label, actionPressed(action));

      if (logic->transientCounterValue(label) >= 3) {
        return true;
      }
    }
    return false;
  }

  void Input::addActionKey(std::string action, std::string key) {
    action_to_key[action] = key;
  }

  void Input::deleteAction(std::string action) {
    action_to_key.erase(action);
  }
}