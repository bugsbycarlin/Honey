/*

  Honey
  Copyright 2018 - Matthew Carlin

  Graphics class wraps OpenGL for drawing and handles image loading.
*/

#ifndef HONEY_GRAPHICS_H_
#define HONEY_GRAPHICS_H_

#include <math.h>
#include <cstdlib>
#include <string>

#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "window.h"

namespace Honey {
  class Graphics {
   public:
      Graphics();

      Window* window;

      void initialize(Window* window);
      void initializeOpenGL();

      void clearScreen(std::string color);

      void updateDisplay();
  };

  extern Graphics* graphics;
}
#endif