/*

  Honey
  Copyright 2019 - Matthew Carlin

*/

#include "menu.h"

using namespace std;

namespace Honey {
  int Menu::unique_count = 0;

  Menu::Menu(
    int x,
    int y,
    int width,
    int height,
    string image_location,
    string image_root,
    int margin_x,
    int margin_y,
    int num_lines,
    int wrap_length,
    bool typewriter,
    float typewriter_delay,
    string font_path,
    int font_size,
    string font_color
  ) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->margin_x = margin_x;
    this->margin_y = margin_y;
    this->num_lines = num_lines;
    this->wrap_length = wrap_length;
    this->typewriter = typewriter;
    this->typewriter_delay = typewriter_delay;

    this->image = image_root + "_" + to_string(unique_count);
    unique_count++;

    text_lines.resize(num_lines);

    // Load the components to surfaces
    map<string, SDL_Surface*> images;
    vector<string> components = {
      "border_top_left",
      "border_left",
      "border_bottom_left",
      "border_top",
      "center",
      "border_bottom",
      "border_top_right",
      "border_right",
      "border_bottom_right",
    };
    for (string component: components) {
      string path = image_location + image_root + "_" + component + ".png";
      images[component] = IMG_Load(path.c_str());
      if (images[component] == nullptr) {
        printf("IMG_Load failed for %s with error: %s\n", path.c_str(), IMG_GetError());
        exit(1);
      }
    }

    // Create a surface for the final menu
    Uint32 rmask, gmask, bmask, amask;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    amask = 0xff000000;

    SDL_Surface* menu_surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);

    // Prepare the menu surface for writing
    SDL_LockSurface(menu_surface);
    unsigned int *menu_surface_ptr = (unsigned int*)menu_surface->pixels;

    unsigned int *component_ptr;

    // Do the top left component
    component_ptr = (unsigned int*)images["border_top_left"]->pixels;
    for (int k = 0; k < images["border_top_left"]->w; k++) {
      for (int l = 0; l < images["border_top_left"]->h; l++) {
        menu_surface_ptr[l * menu_surface->w + k] = component_ptr[l * images["border_top_left"]->w + k];
      }
    }

    // Do the left component
    component_ptr = (unsigned int*)images["border_left"]->pixels;
    for (int k = 0; k < images["border_left"]->w; k++) {
      for (int l = images["border_top_left"]->h; l < menu_surface->h - images["border_bottom_left"]->h; l++) {
        menu_surface_ptr[l * menu_surface->w + k] = component_ptr[(l % images["border_left"]->h) * images["border_left"]->w + k];
      }
    }

    // Do the bottom left component
    component_ptr = (unsigned int*)images["border_bottom_left"]->pixels;
    for (int k = 0; k < images["border_bottom_left"]->w; k++) {
      for (int l = 0; l < images["border_bottom_left"]->h; l++) {
        menu_surface_ptr[(l + menu_surface->h - images["border_bottom_left"]->h) * menu_surface->w + k] = component_ptr[l * images["border_bottom_left"]->w + k];
      }
    }

    // Do the top component
    component_ptr = (unsigned int*)images["border_top"]->pixels;
    for (int k = images["border_top_left"]->w; k < menu_surface->w - images["border_top_right"]->w; k++) {
      for (int l = 0; l < images["border_top"]->h; l++) {
        menu_surface_ptr[l * menu_surface->w + k] = component_ptr[l * images["border_top"]->w + (k % images["border_top"]->w)];
      }
    }

    // Do the center component
    component_ptr = (unsigned int*)images["center"]->pixels;
    for (int k = images["border_top_left"]->w; k < menu_surface->w - images["border_top_right"]->w; k++) {
      for (int l = images["border_top_left"]->h; l < menu_surface->h - images["border_bottom_left"]->h; l++) {
        menu_surface_ptr[l * menu_surface->w + k] = component_ptr[(l % images["center"]->h) * images["center"]->w + (k % images["center"]->w)];
      }
    }

    // Do the bottom component
    component_ptr = (unsigned int*)images["border_bottom"]->pixels;
    for (int k = images["border_bottom_left"]->w; k < menu_surface->w - images["border_bottom_right"]->w; k++) {
      for (int l = 0; l < images["border_bottom"]->h; l++) {
        menu_surface_ptr[(l + menu_surface->h - images["border_bottom"]->h) * menu_surface->w + k] = component_ptr[l * images["border_bottom"]->w + (k % images["border_bottom"]->w)];
      }
    }

    // Do the top right component
    component_ptr = (unsigned int*)images["border_top_right"]->pixels;
    for (int k = 0; k < images["border_top_right"]->w; k++) {
      for (int l = 0; l < images["border_top_right"]->h; l++) {
        menu_surface_ptr[l * menu_surface->w + k + menu_surface->w - images["border_top_right"]->w] = component_ptr[l * images["border_top_right"]->w + k];
      }
    }

    // Do the right component
    component_ptr = (unsigned int*)images["border_right"]->pixels;
    for (int k = 0; k < images["border_right"]->w; k++) {
      for (int l = images["border_top_right"]->h; l < menu_surface->h - images["border_bottom_right"]->h; l++) {
        menu_surface_ptr[l * menu_surface->w + k + menu_surface->w - images["border_right"]->w] = component_ptr[(l % images["border_right"]->h) * images["border_right"]->w + k];
      }
    }

    // Do the bottom right component
    component_ptr = (unsigned int*)images["border_bottom_right"]->pixels;
    for (int k = 0; k < images["border_bottom_right"]->w; k++) {
      for (int l = 0; l < images["border_bottom_right"]->h; l++) {
        menu_surface_ptr[(l + menu_surface->h - images["border_bottom_right"]->h) * menu_surface->w + k + menu_surface->w - images["border_bottom_right"]->w] = component_ptr[l * images["border_bottom_right"]->w + k];
      }
    }
    
    // Close the menu surface and send it on to graphics
    SDL_UnlockSurface(menu_surface);
    graphics.addImageFromSurface(image, menu_surface);

    // Free all the component images
    for (auto const& component_image : images) {
      SDL_FreeSurface((SDL_Surface*) component_image.second);
    }

    // Make the textboxes
    for (int i = 0; i < num_lines; i++) {
      textboxes.push_back(new Textbox(
        font_path,
        font_size,
        " ",
        (position) {x + margin_x, y + margin_y + i * (font_size + 3)},
        font_color
      ));
    }
  }

  void Menu::setText(string text) {
    text_length = text.length();
    std::vector<std::string> words;
    boost::split(words, text, boost::is_any_of(" "), boost::token_compress_on);
    int line_number = 0;
    string segment = "";
    for (int i = 0; i < words.size(); i++) {
      if (segment.length() + words[i].length() + 1 <= wrap_length) {
        segment += words[i] + " ";
      } else if (line_number < num_lines) {
        text_lines[line_number] = segment;
        line_number++;
        segment = words[i] + " ";
      }
    }
    if (line_number < num_lines) {
      text_lines[line_number] = segment;
    }
    if (!typewriter) {
      makeTextboxes();
    }
  }

  void Menu::setTextLines(vector<string> lines) {
    text_length = 0;
    for (int i = 0; i < num_lines; i++) {
      if (i < lines.size()) {
        text_lines[i] = lines[i];
        text_length += lines[i].length();
      } else {
        text_lines[i] = " ";
      }
    }
    if (!typewriter) {
      makeTextboxes();
    }
  }

  void Menu::makeTextboxes() {
    if (typewriter) {
      int remaining_characters = typewriter_position;
      for (int i = 0; i < num_lines; i++) {
        string message = "";
        if (remaining_characters > text_lines[i].length()) {
          message = text_lines[i];
        } else {
          message = text_lines[i].substr(0, remaining_characters);
        }
        remaining_characters -= message.length();
        if (message == "") message = " ";
        textboxes[i]->setText(message);
      }
    } else {
      for (int i = 0; i < num_lines; i++) {
        textboxes[i]->setText(text_lines[i]);
      }
    }
  }

  void Menu::draw() {
    if (typewriter && !typewriterFinished() && timing.since(image + "_typewriter") > typewriter_delay) {
      typewriter_position += 1;
      makeTextboxes();
      timing.mark(image + "_typewriter");
    }
    graphics.setColor("#FFFFFF", 1);
    graphics.drawImage(image, x, y);
    for (int i = 0; i < num_lines; i++) {
      textboxes[i]->draw();
    }
  }

  void Menu::startTypewriter() {
    typewriter_position = 0;
    timing.mark(image + "_typewriter");
  }

  void Menu::finishTypewriter() {
    typewriter_position = text_length;
    makeTextboxes();
  }

  bool Menu::typewriterFinished() {
    return (!typewriter || typewriter_position >= text_length);
  }

  Menu::~Menu() {
  }
}
