#include <string>

#include "window.h"
#include "logic.h"
#include "input.h"
#include "graphics.h"
#include "effects.h"

using namespace Honey;

int main(int argc, char* args[]) {
  printf("Honey Engine, reporting for duty!\n");

  Window* window = new Window("Honey Engine", 960, 600, false);

  graphics->initialize(window);

  // Load three images of bears
  graphics->addImage("explorer_bear", "Art/explorer_bear.png");
  graphics->addImage("grim_bear", "Art/grim_bear.png");
  graphics->addImage("lawn_dart_bear", "Art/lawn_dart_bear.png");

  // Constants to determine the carousel
  std::string bears[] = {"explorer_bear", "grim_bear", "lawn_dart_bear"};
  const int bear_margin = 340; // It's 340 pixels between bears.
  const float animation_duration = 0.5; // The animations will take 0.5 seconds
  
  // Variables to track which bear is where and which animation is which.
  int first_bear = 0; // Mod 3
  int animation_direction = 0;

  input->addActionKey("select left", "left");
  input->addActionKey("select right", "right");
  input->addActionKey("choose", "up");

  bool quit = false;
  while (!quit) {
    input->processInput();

    // If user slides left, switch the bears, and make animations.
    if (input->actionDown("select left") && !logic->isTimeLocked("movement")) {
      logic->makeTimeLock("movement", animation_duration);
      first_bear += 1;
      first_bear = first_bear % 3;
      animation_direction = -1;
      effects->makeTween("slide_bear_0", bear_margin, 0, animation_duration);
      effects->makeTween("slide_bear_1", bear_margin, 0, animation_duration);
      effects->makeTween("slide_bear_2", -2 * bear_margin, 0, animation_duration);
      effects->makeTween("dip_bear_2", 0, 15, animation_duration);
    }

    // If the user slides right, switch the bears, and make animations.
    if (input->actionDown("select right") && !logic->isTimeLocked("movement")) {
      logic->makeTimeLock("movement", animation_duration);
      first_bear -= 1;
      first_bear = (first_bear + 3) % 3;
      animation_direction = 1;
      effects->makeTween("slide_bear_0", 2 * bear_margin, 0, animation_duration);
      effects->makeTween("slide_bear_1", -bear_margin, 0, animation_duration);
      effects->makeTween("slide_bear_2", -bear_margin, 0, animation_duration);
      effects->makeTween("dip_bear_0", 0, 15, animation_duration);
    }

    // If the users presses the choose button, shake the middle bear.
    if (input->actionDown("choose") && !logic->isTimeLocked("movement")) {
      logic->makeTimeLock("movement", animation_duration);
      effects->makeShake("shakey shakey", 10, animation_duration);
      animation_direction = 0;
    }

    if (input->keyPressed("quit") > 0) {
      quit = true;
    }

    if (input->threeQuickKey("escape")) {
      quit = true;
    }

    // Clear the screen to a soft white color
    graphics->clearScreen("#F0F0F0");

    // Switch to 2D drawing mode
    graphics->draw2D();

    for (int i = 0; i <= 2; i++) {
      // Set the basic bear position
      float x = 60 + i * bear_margin;
      float y = 300;
      // Add movement tweens if we're moving
      if (logic->isTimeLocked("movement")) {
        x += effects->tween("slide_bear_" + std::to_string(i), effects->SIGMOID);
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