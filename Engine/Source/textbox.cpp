/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "textbox.h"

namespace Honey {
  Textbox::Textbox(std::string font_path, int font_size, std::string text, std::string color, int x, int y) {
    font = TTF_OpenFont(font_path.c_str(), font_size);

    if (font == nullptr) {
      printf("Error: Failed to load font at path %s.\n", font_path.c_str());
      exit(1);
    }

    this->x = x;
    this->y = y;

    this->text = text;

    intColor c = graphics->parseIntColor(color);
    this->color = {(Uint8) c.r, (Uint8) c.g, (Uint8) c.b};

    // A somewhat unique label to reference this textbox from graphics.
    this->label = font_path + "," + std::to_string(font_size) + "," + std::to_string(x) + "," + std::to_string(y);

    remakeBox();
  }

  void Textbox::setText(std::string text) {
    this->text = text;

    remakeBox();
  }

  void Textbox::setColor(std::string color) {
    intColor c = graphics->parseIntColor(color);
    this->color = {(Uint8) c.r, (Uint8) c.g, (Uint8) c.b};
    remakeBox();
  }

  void Textbox::remakeBox() {
    if (graphics->checkImage(label)) {
      graphics->destroyImage(label);
    }
    text_surface = TTF_RenderText_Blended(font, text.c_str(), color);
    graphics->addImageFromSurface(label, text_surface);
  }

  void Textbox::draw() {
    graphics->drawImage(this->label, x, y);
  }

  void Textbox::destroy() {
    graphics->destroyImage(this->label);
    TTF_CloseFont(font);
  }
}