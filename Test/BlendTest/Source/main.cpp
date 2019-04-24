/*
  BlendTest
*/

#include "honey.h"

#include <string>

using namespace Honey;
using namespace std;

bool quit;

void initialize() {
  quit = false;

  graphics.addImages("Art/", {
    "red",
    "blue",
    "ipsum"
  });
}

void render() {
  graphics.draw2D();
  graphics.clearScreen("#FFFFFF");
  graphics.drawImage("ipsum", 0, 0);
  graphics.drawImage("red", 0, 0);
  graphics.drawImage("blue", 0, 0);
}

void logic() {
  if (input.threeQuickKey("escape")) {
    quit = true;
  }
}

void cleanup() {
  printf("Clean up, clean up, everybody everywhere. Clean up, clean up, everybody do a share.\n");
}

int main(int argc, char* args[]) {
  StartHoney("BlendTest");

  // A reasonable thing to do:
  initialize();
  while (!quit) {
    input.processInput();
    logic();
    render();
    graphics.updateDisplay();
  }
  cleanup();
}

