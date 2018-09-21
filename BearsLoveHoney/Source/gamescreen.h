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

class GameScreen : public Screen {
 public:
  GameScreen(string left_bear, string right_bear, string middle_bear, string player_color);

  void loop();

  void initialize();
  void initializeAssets();
  void initializeLogic();

  void render();
  void logic();

  ~GameScreen();

  string left_bear_string;
  string right_bear_string;
  string middle_bear_string;

  string player_color;

  Sprite* left_bear;
  Sprite* right_bear;
  Sprite* middle_bear;
  Sprite* honeycomb;

  float middle_bear_velocity;

  int player_score;
  int computer_score;
  bool last_busy;

  Textbox* player_score_box;
  Textbox* computer_score_box;

  array<position, 20> star_field;
  Sprite* star_sprite;
};