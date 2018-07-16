#include <string>

#include "window.h"
#include "logic.h"
#include "hotconfig.h"
#include "input.h"
#include "graphics.h"
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

  // Load three images of bears
  graphics->addImage("explorer_bear", "Art/explorer_bear.png");
  graphics->addImage("grim_bear", "Art/grim_bear.png");
  graphics->addImage("lawn_dart_bear", "Art/lawn_dart_bear.png");

  // Constants to determine the carousel
  std::string bears[] = {"explorer_bear", "grim_bear", "lawn_dart_bear"};

  // Variables to track which bear is where and which animation is which.
  int first_bear = 0; // Mod 3
  int animation_direction = 0;

  input->addActionKey("select left", hot_config->getString("input", "select_left_key"));
  input->addActionKey("select right", hot_config->getString("input", "select_right_key"));
  input->addActionKey("choose", hot_config->getString("input", "choose_key"));

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
    int tween_type = hot_config->getInt("animation", "tween_type");
    float dip_height = hot_config->getFloat("animation", "dip_height");
    float shake_width = hot_config->getFloat("animation", "shake_width");
    float lock_duration = hot_config->getFloat("input", "lock_duration");

    input->processInput();

    // If user slides left, switch the bears, and make animations.
    if (input->actionDown("select left") && !logic->isTimeLocked("movement")) {
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
      logic->makeTimeLock("movement", lock_duration);
      effects->makeShake("shakey shakey", shake_width, animation_duration);
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
      // Draw the dang ol' bear.
      graphics->drawImage(bears[(first_bear + i) % 3], x, y);
    }

    // Put everything we've drawn on screen
    graphics->updateDisplay();
  }

  // Get rid of the bears. Throw those bears down the garbage chute!
  graphics->destroyImage("explorer_bear");
  graphics->destroyImage("grim_bear");
  graphics->destroyImage("lawn_dart_bear");

  window->destroy();
}