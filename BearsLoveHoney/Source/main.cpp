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

// Method definitions
void initialize();
void initializeAssets();
void initializeLogic();
void initializeEffects();
void initializeInput();

void render();
void logic();

void cleanup();


// Globals
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

bool quit;


// Functions
int main(int argc, char* args[]) {

  initialize();

  // Start playing music
  sound.playMusic("background_music", -1);

  quit = false;
  while (!quit) {
    logic();
    render();
  }

  cleanup();
}


void initialize() {
  StartHoney("Honey Engine");

  initializeLogic();
  initializeAssets();
  initializeInput();
}


void initializeAssets() {

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

  sound.setSoundVolume(hot_config.getFloat("sound", "sound_volume"));
  sound.setMusicVolume(hot_config.getFloat("sound", "music_volume"));

  // Make bear sprites
  for (int i = 0; i < bear_sprites.size(); i++) {
    bear_colors[i] = hot_config.getString("layout", "bear_color_" + to_string(i));
    bear_sprites[i] = new Sprite(
      bears[i],
      layouts.tileWrap("bearousel", i),
      "#ffffff", 1.0, 0.0, 1.0
    );
  }

  // Add textbox for the selected bear
  bear_name_text = new Textbox(
    "Fonts/jennifer.ttf",
    hot_config.getInt("layout", "font_size"),
    bear_names[1],
    bear_colors[1],
    hot_config.getInt("layout", "font_x"),
    hot_config.getInt("layout", "font_y")
  );

  // Make star sprite
  star_sprite = new Sprite(
    "star", 
    origin,
    "#ffffff", 0.0, 0.0, 0.15
  );
}


void initializeLogic() {
  // Add bear layout; we wrap around every 3 bears
  layouts.makeTileWrapLayout("bearousel",
    hot_config.getInt("layout", "first_bear_x"),
    hot_config.getInt("layout", "first_bear_y"),
    hot_config.getInt("layout", "bear_margin_x"),
    hot_config.getInt("layout", "bear_margin_y"),
    3
  );

  // Arrays of bears and bear names and bear colors
  bears = {"boss_bear", "explorer_bear", "grim_bear", "lawn_dart_bear", "emo_bear", "magnet_bear"};
  bear_names = {"Bossy", "Colonel", "Grimsby", "Darty", "Emo Bob", "Maggie"};

  // Star field locations
  int screen_width = hot_config.getInt("layout", "screen_width");
  int screen_height = hot_config.getInt("layout", "screen_height");
  for (int i = 0; i < star_field.size(); i++) {
    position p {
      .x = math_utils.randomInt(-screen_width / 2.0, 1.5 * screen_width),
      .y = math_utils.randomInt(-screen_height / 2.0, 1.5 * screen_height)
    };
    star_field[i] = p;
  }

  // Variables to track which bear is where and which animation is which.
  selected_bear = 1;
  animation_direction = 0;

  animation_duration = hot_config.getFloat("animation", "animation_duration");
  choose_duration = hot_config.getFloat("animation", "choose_duration");
  shake_width = hot_config.getFloat("animation", "shake_width");
}


void initializeInput() {
  // Set action keys
  input.addActionKey("select left", hot_config.getString("input", "select_left_key"));
  input.addActionKey("select right", hot_config.getString("input", "select_right_key"));
  input.addActionKey("choose", hot_config.getString("input", "choose_key"));
}


void logic() {
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

    // Add bear animations
    for (int i = 0; i < bear_sprites.size(); i++) {
      bear_sprites[i]->setPosition(
        layouts.tileWrap("bearousel", (i + bears.size() - selected_bear + 1) % bears.size()),
        animation_duration
      );
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

    // Add bear animations
    for (int i = 0; i < bears.size(); i++) {
      bear_sprites[i]->setPosition(
        layouts.tileWrap("bearousel", (i + bears.size() - selected_bear + 1) % bears.size()),
        animation_duration
      );
    }
  }

  // If the user presses the choose button, shake the middle bear.
  if (input.actionDown("choose")) {
    position star_travel;
    if (selected_bear != 4) {
      sound.playSound("choose_" + to_string(math_utils.randomInt(1, 5)), 1);
      star_travel.x = 300;
      star_travel.y = -300;
    } else {
      sound.playSound("bob", 1);
      star_travel.x = 0;
      star_travel.y = 300;
    }
    input.lockInput(animation_duration);
    animation_direction = 0;

    // Add star field animations
    star_sprite->setPosition(star_travel, animation_duration);
    star_sprite->fadeInOut(animation_duration);
    star_sprite->setColor(bear_colors[selected_bear]);

    // Add bear shake
    bear_sprites[selected_bear]->shakePosition(
      shake_width,
      choose_duration
    );
  }

  if (input.keyPressed("quit") > 0) {
    quit = true;
  }

  if (input.threeQuickKey("escape")) {
    quit = true;
  }

  if (!input.locked()) {
    star_sprite->setPosition(position {.x = 0, .y = 0});
  }
}


void render() {
  // Clear the screen to a soft white color
  graphics.clearScreen(hot_config.getString("layout", "screen_color"));

  // Switch to 2D drawing mode
  graphics.draw2D();

  // Draw the stars, if necessary
  if (input.locked() && animation_direction == 0) {
    for (int i = 0; i < star_field.size(); i++) {
      star_sprite->draw(star_field[i]);
    }
  }

  // Draw the bears
  for (int i = 0; i < bear_sprites.size(); i++) {
    bear_sprites[i]->draw();
  }

  // Draw the selector
  graphics.setColor(bear_colors[selected_bear], 1);
  position p = layouts.tileWrap("bearousel", 1);
  graphics.drawImage("selector", p.x, p.y, true, 0, 1);
  graphics.setColor("#FFFFFF", 1);

  // Draw the text box
  bear_name_text->draw();

  // Put everything we've drawn on screen
  graphics.updateDisplay();
}


void cleanup() {
  delete bear_name_text;
  for (int i = 0; i < bears.size(); i++) {
    delete bear_sprites[i];
  }
  delete star_sprite;
}