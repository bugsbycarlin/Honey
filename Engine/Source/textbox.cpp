/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "textbox.h"

using namespace std;

namespace Honey {
  Textbox::Textbox(string font_path, int font_size, string text, position pos, string color) : Sprite("placeholder", pos, color, 1, 0, 1) {
    this->draw_label = this->unique_label;

    this->centered = false;

    font = TTF_OpenFont(font_path.c_str(), font_size);

    if (font == nullptr) {
      printf("Error: Failed to load font at path %s.\n", font_path.c_str());
      exit(1);
    }

    this->text = text;

    intColor c = graphics.parseIntColor(color);
    this->text_color = {(Uint8) c.r, (Uint8) c.g, (Uint8) c.b};

    remakeBox();
  }

  int Textbox::getWidth() {
    return width;
  }

  int Textbox::getHeight() {
    return height;
  }

  void Textbox::setText(string text) {
    this->text = text;

    remakeBox();
  }

  void Textbox::setColor(string color) {
    this->color = color;
    intColor c = graphics.parseIntColor(color);
    this->text_color = {(Uint8) c.r, (Uint8) c.g, (Uint8) c.b};
    remakeBox();
  }

  void Textbox::remakeBox() {
    if (graphics.checkImage(draw_label)) {
      graphics.removeImage(draw_label);
    }
    text_surface = TTF_RenderText_Blended(font, text.c_str(), text_color);
    width = text_surface->w;
    height = text_surface->h;
    graphics.addImageFromSurface(draw_label, text_surface);
    SDL_FreeSurface(text_surface);
  }

  Textbox::~Textbox() {
    graphics.removeImage(this->draw_label);
    TTF_CloseFont(font);
  }
}
