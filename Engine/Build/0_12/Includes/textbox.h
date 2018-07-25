/*

  Honey
  Copyright 2018 - Matthew Carlin

  Textbox class holds a textbox of a particular size and font, and prints it to the screen.
*/

#ifndef HONEY_TEXTBOX_H_
#define HONEY_TEXTBOX_H_

// SDL, for window, user input, and media
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

// Standard libraries
#include <stdio.h>
#include <cstdlib>
#include <string>

// Honey includes
#include "primitives.h"
#include "graphics.h"

namespace Honey {
  class Textbox {
   public:
    SDL_Surface* text_surface;
    TTF_Font* font;
    SDL_Color color;

    std::string label;
    std::string text;

    int x;
    int y;  

    Textbox(std::string font_path, int font_size, std::string text, std::string color, int x, int y);

    void setText(std::string text);
    void setColor(std::string color);

    void remakeBox();

    void draw();

    void destroy();
  };
}
#endif
