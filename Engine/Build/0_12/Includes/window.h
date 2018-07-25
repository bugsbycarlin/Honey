/*

  Honey
  Copyright 2018 - Matthew Carlin

  Window class wraps SDL window creation.
*/

#ifndef HONEY_WINDOW_H_
#define HONEY_WINDOW_H_

#include <string>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <OpenGL/gl3.h>

namespace Honey {
  class Window {
   public:
      // The SDL window
      SDL_Window* window;

      // The OpenGL context
      SDL_GLContext context;

      int width;
      int height;

      Window(std::string title, int screen_width, int screen_height, bool fullscreen);

      void destroy();
  };
}
#endif
