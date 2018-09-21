/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#pragma once

#include <string>
#include <array>

#include "honey.h"

using namespace Honey;
using namespace std;

class SelectionScreen : public Screen {
 public:
  SelectionScreen();

  void loop();

  void initialize();
  void initializeAssets();
  void initializeLogic();
  void initializeEffects();
  void initializeInput();

  void render();
  void logic();

  ~SelectionScreen();

  array<string, 6> bears;
  array<string, 6> bear_names;
  array<string, 6> bear_colors;
  array<Sprite*, 6> bear_sprites;
  Textbox* bear_name_text;
  array<position, 120> star_field;
  Sprite* star_sprite;

  int selected_bear;
  int animation_direction;

  float animation_duration;
  float choose_duration;
  float shake_width;
};