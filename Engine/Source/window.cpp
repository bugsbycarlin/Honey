/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "window.h"

using namespace std;

namespace Honey {
  Window& Window::instance() {
    static Window window_instance;
    return window_instance;
  }

  Window::Window() {
  }

  void Window::initialize(string title, int screen_width, int screen_height, bool full_screen) {
    this->width = screen_width;
    this->height = screen_height;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER|SDL_INIT_AUDIO) < 0) {
      printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
      exit(1);
    }

    // Set OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Set the flags
    int flags = SDL_WINDOW_OPENGL;
    if (full_screen) {
      flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
    }

    // Create the window
    window = SDL_CreateWindow(title.c_str(),
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      screen_width, screen_height,
      flags);
    if (window == nullptr) {
      printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
      exit(1);
    }

    // Create the OpenGL context
    context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
      printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
      exit(1);
    }

    // Initialize SDL2_ttf for fonts
    if (TTF_Init() < 0) {
      printf("Warning: Unable to initialize SDL2_ttf for fonts: %s\n", TTF_GetError());
      exit(1);
    }

    // Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
      printf("Unable to set VSync! SDL Error: %s\n", SDL_GetError());
      exit(1);
    }

    // Initialize Audio
    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        printf("Could not initialize SDL mixer! Error: %s\n", Mix_GetError());
        exit(1);
    }
  }

  Window::~Window() {
    // Shut down the SDL_Mixer
    Mix_Quit();

    // Shut down the SDL_TTF fonts
    TTF_Quit();

    // Shut down SDL_image
    IMG_Quit();

    // Destroy the OpenGL context
    SDL_GL_DeleteContext(context);

    // Destroy the window
    SDL_DestroyWindow(window);

    // Shut down the SDL
    SDL_Quit();
  }
}
