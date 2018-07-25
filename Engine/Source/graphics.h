/*

  Honey
  Copyright 2018 - Matthew Carlin

  Graphics class wraps OpenGL for drawing and handles image loading.
*/

#ifndef HONEY_GRAPHICS_H_
#define HONEY_GRAPHICS_H_

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

namespace Honey {
  class Graphics {
   public:
      Graphics();

      Window* window;

      // Pointers to buffers for stuff on the graphics card
      std::unordered_map<std::string, GLuint> vertex_buffers;
      GLuint rectangle_texture_buffer;

      // Maps to store texture information
      std::map<std::string, GLuint> texture_map;
      std::map<std::string, int> texture_widths;
      std::map<std::string, int> texture_heights;

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
      std::vector<glm::mat4> model_stack;

      void initialize(Window* window);
      void initializeOpenGL();
      void initializeBuffersAndGeometry();
      void initializeShaders();

      void pushModelMatrix();
      void popModelMatrix();
      void translate(float x, float y, float z);
      void rotate(float angle, float x, float y, float z);
      void scale(float x, float y, float z);

      floatColor parseFloatColor(std::string color);
      intColor parseIntColor(std::string color);
      void setColor(std::string color, float opacity);
      void setColor(float r, float g, float b, float opacity);

      void clearScreen(std::string color);
      void draw2D();
      void cacheRectangle(float width, float height);
      void drawRectangle(float x_position, float y_position, float width, float height);

      void addImage(std::string label, std::string path);
      void addImageFromSurface(std::string label, SDL_Surface* image);
      void setImage(std::string label);
      void drawImage(std::string label, int x_position, int y_position);
      void drawImage(std::string label, int x_position, int y_position, bool centered, float rotation, float scale);
      void destroyImage(std::string label);
      void destroyAllImages();
      bool checkImage(std::string label);

      void updateDisplay();
  };

  extern Graphics* graphics;
}
#endif