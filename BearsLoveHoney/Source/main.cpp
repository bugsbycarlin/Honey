#include <string>

#include "window.h"
#include "graphics.h"

int main(int argc, char* args[]) {
  printf("Honey Engine, reporting for duty!\n");

  Honey::Window* window = new Honey::Window("Honey Engine", 960, 600, false);

  Honey::graphics->initialize(window);

  // Load three images of bears
  Honey::graphics->addImage("explorer_bear", "Art/explorer_bear.png");
  Honey::graphics->addImage("grim_bear", "Art/grim_bear.png");
  Honey::graphics->addImage("lawn_dart_bear", "Art/lawn_dart_bear.png");

  // Clear the screen to a soft white color
  Honey::graphics->clearScreen("#F0F0F0");

  // Switch to 2D drawing mode
  Honey::graphics->draw2D();

  // Draw three bears in three different locations
  Honey::graphics->drawImage("explorer_bear", 60, 300);
  Honey::graphics->drawImage("grim_bear", 400, 300);
  Honey::graphics->drawImage("lawn_dart_bear", 740, 300);

  // Put everything we've drawn on screen
  Honey::graphics->updateDisplay();

  // Get rid of the bears. Throw those bears down the garbage chute!
  Honey::graphics->destroyImage("explorer_bear");
  Honey::graphics->destroyImage("grim_bear");
  Honey::graphics->destroyImage("lawn_dart_bear");

  SDL_Delay(3000);

  window->destroy();
}