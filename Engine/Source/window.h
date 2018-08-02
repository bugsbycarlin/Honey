/*

  Honey
  Copyright 2018 - Matthew Carlin

  Window class wraps SDL window creation.
*/

#pragma once

#include <string>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <OpenGL/gl3.h>

using namespace std;

namespace Honey {
  class Window {
   public:
      // The SDL window
      SDL_Window* window;

      // The OpenGL context
      SDL_GLContext context;

      int width;
      int height;

      Window();

      void initialize(string title, int screen_width, int screen_height, bool fullscreen);

      ~Window();
  };

  extern Window* window;
}
