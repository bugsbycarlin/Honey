#include <string>

#include "window.h"
#include "graphics.h"

int main(int argc, char* args[]) {
  printf("Honey Engine, reporting for duty!\n");

  Honey::Window* window = new Honey::Window("Honey Engine", 960, 600, false);

  Honey::graphics->initialize(window);

  // Clear the screen to a soft white color
  Honey::graphics->clearScreen("#F0F0F0");

  // Put everything we've drawn on screen
  Honey::graphics->updateDisplay();

  SDL_Delay(3000);

  window->destroy();
}