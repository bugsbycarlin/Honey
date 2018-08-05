/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "textbox.h"

using namespace std;

namespace Honey {
  Textbox::Textbox(string font_path, int font_size, string text, string color, int x, int y) {
    font = TTF_OpenFont(font_path.c_str(), font_size);

    if (font == nullptr) {
      printf("Error: Failed to load font at path %s.\n", font_path.c_str());
      exit(1);
    }

    this->x = x;
    this->y = y;

    this->text = text;

    intColor c = graphics.parseIntColor(color);
    this->color = {(Uint8) c.r, (Uint8) c.g, (Uint8) c.b};

    // A somewhat unique label to reference this textbox from graphics.
    this->label = font_path + "," + to_string(font_size) + "," + to_string(x) + "," + to_string(y);

    remakeBox();
  }

  void Textbox::setText(string text) {
    this->text = text;

    remakeBox();
  }

  void Textbox::setColor(string color) {
    intColor c = graphics.parseIntColor(color);
    this->color = {(Uint8) c.r, (Uint8) c.g, (Uint8) c.b};
    remakeBox();
  }

  void Textbox::remakeBox() {
    if (graphics.checkImage(label)) {
      graphics.removeImage(label);
    }
    text_surface = TTF_RenderText_Blended(font, text.c_str(), color);
    width = text_surface->w;
    height = text_surface->h;
    graphics.addImageFromSurface(label, text_surface);
    SDL_FreeSurface(text_surface);
  }

  void Textbox::draw() {
    graphics.drawImage(this->label, x, y);
  }

  Textbox::~Textbox() {
    graphics.removeImage(this->label);
    TTF_CloseFont(font);
  }
}
