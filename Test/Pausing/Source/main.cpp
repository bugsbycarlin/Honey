/*
  Pausing Test Program
  Matthew Carlin
  Copyright 2018
*/

#include <string>
#include <array>

#include "honey.h"

using namespace Honey;
using namespace std;

void initialize();
void render();
void logic();

bool paused;

bool quit;

// Functions
int main(int argc, char* args[]) {

  initialize();

  quit = false;
  while (!quit) {
    logic();
    render();
  }
}


void initialize() {
  StartHoney("Pausing Test");

  graphics.addImages("Art/", {
    "ball",
    "paused",
    "overlay",
  });

  effects.makeOscillation("ball", 450, 5);
  effects.start("ball");
}

void logic() {
  input.processInput();

  if (input.keyPressed("p") > 0) {
    if (!paused) {
      paused = true;
      timing.pause();
      effects.makeOscillation("paused", 30, 3);
      effects.start("paused");
    } else {
      paused = false;
      timing.unpause();
    }
  }

  if (input.keyPressed("quit") > 0) {
    quit = true;
  }

  if (input.threeQuickKey("escape")) {
    quit = true;
  }
}

void render() {
  graphics.clearScreen("#DDDDDD");

  // Switch to 2D drawing mode
  graphics.draw2D();

  graphics.drawImage("ball", 450 + effects.oscillation("ball"), 270);

  if (paused) {
    graphics.setColor(1, 1, 1, 0.3);
    graphics.drawImage("overlay", 0, 0);
    graphics.setColor(1, 1, 1, 1);
    graphics.drawImage("paused", 0, effects.oscillation("paused"));
  }

  // Put everything we've drawn on screen
  graphics.updateDisplay();
}

