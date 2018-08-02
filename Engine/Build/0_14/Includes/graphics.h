/*

  Honey
  Copyright 2018 - Matthew Carlin

  Graphics class wraps OpenGL for drawing and handles image loading.
*/

#pragma once

#include <math.h>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "primitives.h"
#include "window.h"
#include "shaders.h"

using namespace std;

namespace Honey {
  class Graphics {
   public:
      Graphics();

      void initialize();

      void pushModelMatrix();
      void popModelMatrix();
      void translate(float x, float y, float z);
      void rotate(float angle, float x, float y, float z);
      void scale(float x, float y, float z);

      floatColor parseFloatColor(string color);
      intColor parseIntColor(string color);
      void setColor(string color, float opacity);
      void setColor(float r, float g, float b, float opacity);

      void clearScreen(string color);
      void draw2D();
      void drawRectangle(float x_position, float y_position, float width, float height);

      void addImage(string label, string path);
      void addImageFromSurface(string label, SDL_Surface* image);
      void setImage(string label);
      void drawImage(string label, int x_position, int y_position);
      void drawImage(string label, int x_position, int y_position, bool centered, float rotation, float scale);
      void destroyImage(string label);
      void destroyAllImages();
      bool checkImage(string label);

      void updateDisplay();

      ~Graphics();

    private:
      // Pointers to buffers for stuff on the graphics card
      unordered_map<string, GLuint> vertex_buffers;
      GLuint rectangle_texture_buffer;
      GLuint vertex_array_id;

      // Maps to store texture information
      map<string, GLuint> texture_map;
      map<string, int> texture_widths;
      map<string, int> texture_heights;

      // Variables for communicating with the graphics card
      GLuint mvp_matrix_id;
      GLuint color_id;
      GLuint texture_sampler_id;
      GLuint shader_program;

      // Variables for working on the CPU size
      glm::mat4 projection;
      glm::mat4 model;
      glm::vec4 color;
      bool using_2d;
      vector<glm::mat4> model_stack;

      void initializeOpenGL();
      void initializeBuffersAndGeometry();
      void initializeShaders();

      void cacheRectangle(float width, float height);
  };

  extern Graphics* graphics;
}
