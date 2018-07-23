#include <string>

#include "window.h"
#include "logic.h"
#include "hotconfig.h"
#include "input.h"
#include "graphics.h"
#include "textbox.h"
#include "sound.h"
#include "effects.h"

using namespace Honey;

int main(int argc, char* args[]) {
  printf("Honey Engine, reporting for duty!\n");

  // Load configuration
  if(hot_config->checkAndUpdate() != hot_config->SUCCESS) {
    exit(1);
  }

  int screen_width = hot_config->getInt("layout", "screen_width");
  int screen_height = hot_config->getInt("layout", "screen_height");
  bool full_screen = hot_config->getBool("layout", "full_screen");

  Window* window = new Window("Honey Engine", screen_width, screen_height, full_screen);

  graphics->initialize(window);
  sound->initialize();

  // Load images
  graphics->addImage("explorer_bear", "Art/explorer_bear.png");
  graphics->addImage("grim_bear", "Art/grim_bear.png");
  graphics->addImage("lawn_dart_bear", "Art/lawn_dart_bear.png");
  graphics->addImage("star", "Art/star.png");

  // Add music and some sound effects
  sound->addMusic("background_music", "Sound/Nothing_to_Fear.mp3");
  sound->addSound("move_left", "Sound/C_Square1.wav");
  sound->addSound("move_right", "Sound/C_Square2.wav");
  sound->addSound("choose_1", "Sound/Chant1.wav");
  sound->addSound("choose_2", "Sound/Chant2.wav");
  sound->addSound("choose_3", "Sound/Chant3.wav");
  sound->addSound("choose_4", "Sound/Chant4.wav");
  sound->addSound("choose_5", "Sound/Chant5.wav");

  // Start playing music
  sound->playMusic("background_music", -1);

  // Constants to determine the carousel
  std::string bears[] = {"explorer_bear", "grim_bear", "lawn_dart_bear"};
  std::string bear_names[] = {"Colonel", "Grimsby", "Darty"};

  // Make text boxes
  int font_margin = hot_config->getInt("layout", "font_margin");
  int font_x = hot_config->getInt("layout", "font_x");
  int font_y = hot_config->getInt("layout", "font_y");
  int font_size = hot_config->getInt("layout", "font_size");
  Textbox* bear_text[3];
  for (int i = 0; i < 3; i++) {
    bear_text[i] = new Textbox("Fonts/jennifer.ttf", font_size, bear_names[i],
      hot_config->getString("layout", "bear_color_" + std::to_string(i)), 
      font_x + i * font_margin, font_y);
  }

  // Variables to track which bear is where and which animation is which.
  int first_bear = 0; // Mod 3
  int animation_direction = 0;

  // Set action keys
  input->addActionKey("select left", hot_config->getString("input", "select_left_key"));
  input->addActionKey("select right", hot_config->getString("input", "select_right_key"));
  input->addActionKey("choose", hot_config->getString("input", "choose_key"));

  // Make a star oscillation effect
  int star_y = hot_config->getInt("layout", "star_y");
  float starbounce_height = hot_config->getFloat("animation", "starbounce_height");
  float starbounce_period = hot_config->getFloat("animation", "starbounce_period");
  effects->makeOscillation("starbounce", starbounce_height, starbounce_period);
  float startilt_angle = hot_config->getFloat("animation", "startilt_angle");
  float startilt_period = hot_config->getFloat("animation", "startilt_period");
  effects->makeOscillation("startilt", startilt_angle, startilt_period);

  // Make a star field
  int star_field_x[60];
  int star_field_y[60];
  for (int i = 0; i < 60; i++) {
    star_field_x[i] = logic->randomInt(-screen_width / 2.0, 1.5 * screen_width);
    star_field_y[i] = logic->randomInt(-screen_height / 2.0, 1.5 * screen_height);
  }

  bool quit = false;
  while (!quit) {
    // Check and load configuration (default every 2 seconds)
    hot_config->checkAndUpdate();

    // Set a bunch of variables from configuration
    int bear_margin = hot_config->getInt("layout", "bear_margin");
    int first_bear_x = hot_config->getInt("layout", "first_bear_x");
    int first_bear_y = hot_config->getInt("layout", "first_bear_y");
    std::string screen_color = hot_config->getString("layout", "screen_color");
    float animation_duration = hot_config->getFloat("animation", "animation_duration");
    float choose_duration = hot_config->getFloat("animation", "choose_duration");
    int tween_type = hot_config->getInt("animation", "tween_type");
    float dip_height = hot_config->getFloat("animation", "dip_height");
    float shake_width = hot_config->getFloat("animation", "shake_width");
    float lock_duration = hot_config->getFloat("input", "lock_duration");
    float sound_volume = hot_config->getFloat("sound", "sound_volume");
    float music_volume = hot_config->getFloat("sound", "music_volume");

    sound->setSoundVolume(sound_volume);
    sound->setMusicVolume(music_volume);

    input->processInput();

    // If user slides left, switch the bears, and make animations.
    if (input->actionDown("select left") && !logic->isTimeLocked("movement")) {
      sound->playSound("move_left", 1);
      logic->makeTimeLock("movement", lock_duration);
      first_bear += 1;
      first_bear = first_bear % 3;
      animation_direction = -1;
      effects->makeTween("slide_bear_0", bear_margin, 0, animation_duration);
      effects->makeTween("slide_bear_1", bear_margin, 0, animation_duration);
      effects->makeTween("slide_bear_2", -2 * bear_margin, 0, animation_duration);
      effects->makeTween("dip_bear_2", 0, dip_height, animation_duration);
    }

    // If the user slides right, switch the bears, and make animations.
    if (input->actionDown("select right") && !logic->isTimeLocked("movement")) {
      sound->playSound("move_right", 1);
      logic->makeTimeLock("movement", lock_duration);
      first_bear -= 1;
      first_bear = (first_bear + 3) % 3;
      animation_direction = 1;
      effects->makeTween("slide_bear_0", 2 * bear_margin, 0, animation_duration);
      effects->makeTween("slide_bear_1", -bear_margin, 0, animation_duration);
      effects->makeTween("slide_bear_2", -bear_margin, 0, animation_duration);
      effects->makeTween("dip_bear_0", 0, dip_height, animation_duration);
    }

    // If the users presses the choose button, shake the middle bear.
    if (input->actionDown("choose") && !logic->isTimeLocked("movement")) {
      sound->playSound("choose_" + std::to_string(logic->randomInt(1,5)), 1);
      logic->makeTimeLock("movement", lock_duration);
      effects->makeShake("shakey shakey", shake_width, choose_duration);
      effects->makeTween("star_phasing", 0, 1, animation_duration);
      animation_direction = 0;
    }

    if (input->keyPressed("quit") > 0) {
      quit = true;
    }

    if (input->threeQuickKey("escape")) {
      quit = true;
    }

    // Clear the screen to a soft white color
    graphics->clearScreen(screen_color);

    // Switch to 2D drawing mode
    graphics->draw2D();

    // Light up the sky
    if (effects->tween("star_phasing", 0) != 0) {
      int center_bear = (first_bear + 1) % 3;
      float opacity = effects->tween("star_phasing", effects->SINEWAVE);
      float shooting_star = 300 * effects->tween("star_phasing", effects->LINEAR);
      graphics->setColor(hot_config->getString("layout", "bear_color_" + std::to_string(center_bear)), opacity);

      for (int i = 0; i < 60; i++) {
        graphics->drawImage("star", star_field_x[i] + shooting_star, star_field_y[i] - shooting_star, true, 0, 0.15);
      }

      graphics->setColor(1, 1, 1, 1);
    }

    // Draw the bears
    for (int i = 0; i <= 2; i++) {
      // Set the basic bear position
      float x = first_bear_x + i * bear_margin;
      float y = first_bear_y;
      // Add movement tweens if we're moving
      if (logic->isTimeLocked("movement")) {
        x += effects->tween("slide_bear_" + std::to_string(i), tween_type);
        y += effects->tween("dip_bear_" + std::to_string(i), effects->SINEWAVE);
      }
      // Add shakey shakey to the middle bear.
      // This will only do anything if the effect is turned on.
      if (i == 1) {
        x += effects->shake("shakey shakey");
        y += effects->shake("shakey shakey");
      }

      int bear_num = (first_bear + i) % 3;

      // Draw the dang ol' bear.
      graphics->drawImage(bears[bear_num], x, y);

      // And the stars
      x += 78;
      y = star_y + effects->oscillation("starbounce");
      if (logic->isTimeLocked("movement")) {
        //x += effects->tween("slide_bear_" + std::to_string(i), tween_type);
        y += effects->tween("dip_bear_" + std::to_string(i), effects->SINEWAVE);
      }
      graphics->setColor(hot_config->getString("layout", "bear_color_" + std::to_string(bear_num)), 1);
      graphics->drawImage("star", x, y, true, effects->oscillation("startilt"), 0.25);
      graphics->setColor("#FFFFFF", 1);

      // And the font
      bear_text[bear_num]->x = font_x + first_bear + i * font_margin;
      bear_text[bear_num]->y = font_y;
      if (logic->isTimeLocked("movement")) {
        bear_text[bear_num]->x += effects->tween("slide_bear_" + std::to_string(i), tween_type);
        bear_text[bear_num]->y += effects->tween("dip_bear_" + std::to_string(i), effects->SINEWAVE);
      }
      bear_text[bear_num]->draw();

    }

    // Put everything we've drawn on screen
    graphics->updateDisplay();
  }

  // Kill the fonts
  bear_text[0]->shutdown();
  bear_text[1]->shutdown();
  bear_text[2]->shutdown();

  // Get rid of the bears. Throw those bears down the garbage chute!
  graphics->destroyImage("explorer_bear");
  graphics->destroyImage("grim_bear");
  graphics->destroyImage("lawn_dart_bear");
  graphics->destroyImage("star");

  // Kill the sounds too!
  sound->destroyMusic("background_music");
  sound->destroySound("move_left");
  sound->destroySound("move_right");

  window->destroy();
}