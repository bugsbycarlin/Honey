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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "window.h"
#include "shaders.h"

namespace Honey {
  class Graphics {
   public:
      Graphics();

      Window* window;

      GLuint mvp_matrix_id;
      GLuint shader_program;

      glm::mat4 projection;
      glm::mat4 model;

      bool using_2d;

      void initialize(Window* window);
      void initializeOpenGL();
      void initializeBuffersAndGeometry();
      void initializeShaders();

      void clearScreen(std::string color);
      void draw2D();
      void drawRectangle(float x_position, float y_position, float width, float height);

      void updateDisplay();
  };

  extern Graphics* graphics;
}
#endif