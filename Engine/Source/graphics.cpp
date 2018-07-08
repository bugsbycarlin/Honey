/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "graphics.h"

namespace Honey {
  Graphics* graphics = new Graphics();

  Graphics::Graphics() {
  }

  void Graphics::initialize(Window* window) {
    this->window = window;

    initializeOpenGL();
  }

  void Graphics::initializeOpenGL() {
    // Print OpenGL version and Shading Language version
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("OpenGL Shading Language Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // glViewport tells OpenGL it's drawing on a square the size of the window.
    glViewport(0, 0, window->width, window->height);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      printf("Error initializing OpenGL! %d\n", error);
      exit(1);
    }
  }

  void Graphics::clearScreen(std::string color) {
    // The clear screen method takes a hex-string color (eg #A4F4E3 or #FFFFFF or #003030)
    // and decomposes it into r, g, and b floats, which are each a fraction from 0 (black)
    // to 1 (fully saturated). (1,0,0) is full red, (0,1,0) is full green, (0,0,1) is full
    // blue, and (1,1,1) is full white.
    float r = std::stoi(color.substr(1,2), 0, 16) / 255.0f;
    float g = std::stoi(color.substr(3,2), 0, 16) / 255.0f;
    float b = std::stoi(color.substr(5,2), 0, 16) / 255.0f;

    // Tell OpenGL to clear the whole screen to our chosen color.
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void Graphics::updateDisplay() {
    // Take everything we've drawn since the last wipe and put it on the screen all at once.
    SDL_GL_SwapWindow(window->window);
  }
}