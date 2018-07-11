#include <string>

#include "window.h"
#include "logic.h"
#include "input.h"
#include "graphics.h"

using namespace Honey;

int main(int argc, char* args[]) {
  printf("Honey Engine, reporting for duty!\n");

  Window* window = new Window("Honey Engine", 960, 600, false);

  graphics->initialize(window);

  // Load three images of bears
  graphics->addImage("explorer_bear", "Art/explorer_bear.png");
  graphics->addImage("grim_bear", "Art/grim_bear.png");
  graphics->addImage("lawn_dart_bear", "Art/lawn_dart_bear.png");

  std::string bears[] = {"explorer_bear", "grim_bear", "lawn_dart_bear"};
  int first_bear = 0; // This can have the value 0, 1, or 2, according to which bear comes first on the screen.
  int bear_margin = 340; // It's 340 pixels between bears.
  int animation_direction = 0; // -1 is a left movement, 1 is a right movement.

  bool quit = false;
  while (!quit) {
    input->processInput();

    input->addActionKey("run left", "left");
    input->addActionKey("run right", "right");

    if (input->actionDown("run left") && !logic->isTimeLocked("movement")) {
      printf("Move left!\n");
      logic->makeTimeLock("movement", 0.5);

      first_bear += 1;
      if (first_bear > 2) {
        first_bear = 0;
      }
      animation_direction = -1;
    }

    if (input->actionDown("run right") && !logic->isTimeLocked("movement")) {
      printf("Move right!\n");
      logic->makeTimeLock("movement", 0.5);
      first_bear -= 1;
      if (first_bear < 0) {
        first_bear = 2;
      }
      animation_direction = 1;
    }

    if (input->keyPressed("quit") > 0) {
      quit = true;
    }

    if (input->keyPressed("escape") > 0) {
      if (logic->transientCounterValue("escape_counter") <= 0) {
        logic->makeTransientCounter("escape_counter", 1.0);
      }

      logic->incrementTransientCounter("escape_counter", input->keyPressed("escape"));

      if (logic->transientCounterValue("escape_counter") >= 3) {
        quit = true;
      }
    }

    // Clear the screen to a soft white color
    graphics->clearScreen("#F0F0F0");

    // Switch to 2D drawing mode
    graphics->draw2D();

    // Draw three bears in three different locations
    float adjustment = 0;
    if (logic->isTimeLocked("movement")) {
      adjustment = animation_direction * bear_margin * (1 - (logic->timeSince("movement") / logic->time_locks["movement"]));
    }
    graphics->drawImage(bears[first_bear], 60 - adjustment, 300);
    graphics->drawImage(bears[(first_bear + 1) % 3], 60 + bear_margin - adjustment, 300);
    graphics->drawImage(bears[(first_bear + 2) % 3], 60 + 2 * bear_margin - adjustment, 300);

    // Put everything we've drawn on screen
    graphics->updateDisplay();
  }

  // Get rid of the bears. Throw those bears down the garbage chute!
  graphics->destroyImage("explorer_bear");
  graphics->destroyImage("grim_bear");
  graphics->destroyImage("lawn_dart_bear");

  window->destroy();
}