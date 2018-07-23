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

    Uint8 r = std::stoi(color.substr(1,2), 0, 16);
    Uint8 g = std::stoi(color.substr(3,2), 0, 16);
    Uint8 b = std::stoi(color.substr(5,2), 0, 16);

    // A somewhat unique label to reference this textbox from graphics.
    this->label = font_path + "," + std::to_string(font_size) + "," + std::to_string(x) + "," + std::to_string(y);

    this->color = {r, g, b};

    text_surface = TTF_RenderText_Blended(font, this->text.c_str(), this->color);
    graphics->addTextImage(this->label, text_surface);
  }


  void Textbox::setText(std::string text) {
    this->text = text;

    text_surface = TTF_RenderText_Blended(font, this->text.c_str(), this->color);
    graphics->destroyImage(this->label);
    graphics->addTextImage(this->label, text_surface);
  }

  void Textbox::setColor(std::string color) {
    Uint8 r = std::stoi(color.substr(1,2), 0, 16);
    Uint8 g = std::stoi(color.substr(3,2), 0, 16);
    Uint8 b = std::stoi(color.substr(5,2), 0, 16);

    this->color = {r, g, b};

    text_surface = TTF_RenderText_Blended(font, this->text.c_str(), this->color);
    graphics->destroyImage(this->label);
    graphics->addTextImage(this->label, text_surface);
  }

  void Textbox::draw() {
    graphics->drawImage(this->label, x, y);
  }

  void Textbox::shutdown() {
    graphics->destroyImage(this->label);
    TTF_CloseFont(font);
  }
}