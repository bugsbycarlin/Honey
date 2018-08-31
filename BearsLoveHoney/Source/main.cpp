/*
  BearsLoveHoney
  Matthew Carlin
  Copyright 2018
*/

#include <string>
#include <array>

#include "honey.h"

using namespace Honey;
using namespace std;

int main(int argc, char* args[]) {
  printf("Honey Engine, reporting for duty!\n");

  StartHoney("Honey Engine");

  int screen_width = hot_config.getInt("layout", "screen_width");
  int screen_height = hot_config.getInt("layout", "screen_height");

  // Load images
  graphics.addImages( "Art/", {
    "boss_bear",
    "explorer_bear",
    "grim_bear",
    "lawn_dart_bear",
    "emo_bear",
    "magnet_bear",
    "star",
    "selector"
  });

  // Add music and some sound effects
  sound.addMusic("background_music", "Sound/Nothing_to_Fear.mp3");
  sound.addSounds( "Sound/", {
    "move_left",
    "move_right",
    "choose_1",
    "choose_2",
    "choose_3",
    "choose_4",
    "choose_5",
    "bob"
  });

  // Arrays of bears and bear names and bear colors
  array<string, 6> bears = {"boss_bear", "explorer_bear", "grim_bear", "lawn_dart_bear", "emo_bear", "magnet_bear"};
  array<string, 6> bear_names = {"Bossy", "Colonel", "Grimsby", "Darty", "Emo Bob", "Maggie"};
  array<string, 6> bear_colors = {
    hot_config.getString("layout", "bear_color_0"),
    hot_config.getString("layout", "bear_color_1"),
    hot_config.getString("layout", "bear_color_2"),
    hot_config.getString("layout", "bear_color_3"),
    hot_config.getString("layout", "bear_color_4"),
    hot_config.getString("layout", "bear_color_5"),
  };

  // Add star field locations
  array<int, 120> star_field_x;
  array<int, 120> star_field_y;
  for (int i = 0; i < star_field_x.size(); i++) {
    star_field_x[i] = math_utils.randomInt(-screen_width / 2.0, 1.5 * screen_width);
    star_field_y[i] = math_utils.randomInt(-screen_height / 2.0, 1.5 * screen_height);
  }

  // Add bear layout; we wrap around every 3 bears
  int first_bear_x = hot_config.getInt("layout", "first_bear_x");
  int first_bear_y = hot_config.getInt("layout", "first_bear_y");
  int bear_margin_x = hot_config.getInt("layout", "bear_margin_x");
  int bear_margin_y = hot_config.getInt("layout", "bear_margin_y");
  layouts.makeTileWrapLayout("bearousel", first_bear_x, first_bear_y, bear_margin_x, bear_margin_y, 3);

  // Add textbox for the selected bear
  int font_x = hot_config.getInt("layout", "font_x");
  int font_y = hot_config.getInt("layout", "font_y");
  int font_size = hot_config.getInt("layout", "font_size");
  Textbox* bear_name_text = new Textbox(
    "Fonts/jennifer.ttf",
    font_size,
    bear_names[1],
    bear_colors[1],
    font_x,
    font_y);

  // Variables to track which bear is where and which animation is which.
  int selected_bear = 1;
  int animation_direction = 0;

  float animation_duration = hot_config.getFloat("animation", "animation_duration");
  float choose_duration = hot_config.getFloat("animation", "choose_duration");
  float shake_width = hot_config.getFloat("animation", "shake_width");

  // Make tweens to animate the bears
  for (int i = 0; i < bears.size(); i++) {
    for (int direction : { -1, 1 }) {
      position p1 = layouts.tileWrap("bearousel", (i + bears.size() + direction) % bears.size());
      position p2 = layouts.tileWrap("bearousel", i);
      effects.makeTween(bears[i] + "_x_" + to_string(direction), p1.x, p2.x, animation_duration);
      effects.makeTween(bears[i] + "_y_" + to_string(direction), p1.y, p2.y, animation_duration);
    }
  }

  // Make tweens for the shaking and star animations
  effects.makeShake("shakey shakey", shake_width, choose_duration);
  effects.makeTween("star_opacity", 0, 1, animation_duration);
  effects.makeTween("star_travel", 0, 300, animation_duration);

  // Set action keys
  input.addActionKey("select left", hot_config.getString("input", "select_left_key"));
  input.addActionKey("select right", hot_config.getString("input", "select_right_key"));
  input.addActionKey("choose", hot_config.getString("input", "choose_key"));

  string screen_color = hot_config.getString("layout", "screen_color");

  float sound_volume = hot_config.getFloat("sound", "sound_volume");
  float music_volume = hot_config.getFloat("sound", "music_volume");

  sound.setSoundVolume(sound_volume);
  sound.setMusicVolume(music_volume);

  // Start playing music
  sound.playMusic("background_music", -1);

  bool quit = false;
  while (!quit) {
    input.processInput();

    // If user slides left, switch the bears, and make animations.
    if (input.actionDown("select left")) {
      sound.playSound("move_left", 1);
      input.lockInput(animation_duration);
      selected_bear -= 1;
      selected_bear = (selected_bear + bears.size()) % bears.size();
      bear_name_text->setColor(bear_colors[selected_bear]);
      bear_name_text->setText(bear_names[selected_bear]);
      animation_direction = -1;

      for (int i = 0; i < bears.size(); i++) {
        effects.start(bears[i] + "_x_" + to_string(animation_direction));
        effects.start(bears[i] + "_y_" + to_string(animation_direction));
      }
    }

    // If the user slides right, switch the bears, and make animations.
    if (input.actionDown("select right")) {
      sound.playSound("move_right", 1);
      input.lockInput(animation_duration);
      selected_bear += 1;
      selected_bear = selected_bear % bears.size();
      bear_name_text->setColor(bear_colors[selected_bear]);
      bear_name_text->setText(bear_names[selected_bear]);
      animation_direction = 1;

      for (int i = 0; i < bears.size(); i++) {
        effects.start(bears[i] + "_x_" + to_string(animation_direction));
        effects.start(bears[i] + "_y_" + to_string(animation_direction));
      }
    }

    // If the user presses the choose button, shake the middle bear.
    if (input.actionDown("choose")) {
      if (selected_bear != 4) {
        sound.playSound("choose_" + to_string(math_utils.randomInt(1, 5)), 1);
      } else {
        sound.playSound("bob", 1);
      }
      input.lockInput(animation_duration);
      effects.start("shakey shakey");
      effects.start("star_opacity");
      effects.start("star_travel");
      animation_direction = 0;
    }

    if (input.keyPressed("quit") > 0) {
      quit = true;
    }

    if (input.threeQuickKey("escape")) {
      quit = true;
    }

    // Clear the screen to a soft white color
    graphics.clearScreen(screen_color);

    // Switch to 2D drawing mode
    graphics.draw2D();

    // Light up the sky
    if (effects.busy("star_opacity")) {
      float opacity = effects.tween("star_opacity", effects.SINEWAVE);
      float star_travel = effects.tween("star_travel", effects.LINEAR);
      graphics.setColor(bear_colors[selected_bear], opacity);

      if (selected_bear != 4) {
        for (int i = 0; i < star_field_x.size(); i++) {
          graphics.drawImage("star", star_field_x[i] + star_travel, star_field_y[i] - star_travel, true, 0, 0.15);
        }
      } else {
        for (int i = 0; i < star_field_x.size(); i++) {
          graphics.drawImage("star", star_field_x[i], star_field_y[i] + star_travel, true, 0, 0.15);
        }
      }

      graphics.setColor(1, 1, 1, 1);
    }

    // Draw the bears!
    for (int i = 0; i < bears.size(); i++) {
      // Shift the chosen bears by the carousel selection value
      int bear_num = (selected_bear - 1 + i + bears.size()) % bears.size();
      position p = layouts.tileWrap("bearousel", i);
      if (input.locked()) {
        if (animation_direction != 0) {
          p.x = effects.tween(bears[i] + "_x_" + to_string(animation_direction), effects.SIGMOID);
          p.y = effects.tween(bears[i] + "_y_" + to_string(animation_direction), effects.SIGMOID);
        } else if (bear_num == selected_bear) {
          p.x += effects.shake("shakey shakey");
          p.y += effects.shake("shakey shakey");
        }
      }
      graphics.drawImage(bears[bear_num], p.x, p.y);
    }

    // Draw the selector
    graphics.setColor(bear_colors[selected_bear], 1);
    position p = layouts.tileWrap("bearousel", 1);
    graphics.drawImage("selector", p.x, p.y);
    graphics.setColor("#FFFFFF", 1);

    // Draw the text box
    bear_name_text->draw();

    // Put everything we've drawn on screen
    graphics.updateDisplay();
  }

  delete bear_name_text;
}
