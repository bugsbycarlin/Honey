/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "window.h"

namespace Honey {
  Window::Window(std::string title, int screen_width, int screen_height, bool fullscreen) {
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER) < 0) {
      printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
      exit(1);
    }

    // Set the flags
    int flags = SDL_WINDOW_OPENGL;
    if (fullscreen) {
      flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
    }

    // Create the window
    window = SDL_CreateWindow(title.c_str(),
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      screen_width, screen_height,
      flags);
    if (window == NULL) {
      printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
      exit(1);
    }

    // Create the OpenGL context
    context = SDL_GL_CreateContext(window);
    if (context == NULL) {
      printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
      exit(1);
    }

    // Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
      printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
      exit(1);
    }
  }

  void Window::destroy() {
    // Destroy the window
    SDL_DestroyWindow(window);
    window = NULL;

    // Shut down the SDL
    SDL_Quit();
  }
}