/*
  Collisions Test Program
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

bool test_collision(int i, int j);

bool quit;

float x_velocity;
float y_velocity;
float acceleration;
float deceleration;

position spots[7];

string pictures[7] = {
  "180_180_line_",
  "180_180_line_",
  "180_90_line_",
  "40_120_box_",
  "160_80_box_",
  "60_circle_",
  "120_circle_",
};

box box1 = {0, 0, 40, 120};
box box2 = {0, 0, 160, 80};

float circle_1_radius = 30.0f;
float circle_2_radius = 60.0f;

int selection = 0;

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
  StartHoney("Collisions Test");

  graphics.addImages("Art/", {
    "180_180_line_black",
    "180_180_line_green",
    "180_90_line_green",
    "180_90_line_black",
    "40_120_box_black",
    "40_120_box_green",
    "160_80_box_green",
    "160_80_box_black",
    "60_circle_black",
    "60_circle_green",
    "120_circle_green",
    "120_circle_black",
  });

  for (int i = 0; i < 7; i++) {
    spots[i] = {math_utils.randomInt(100, 800), math_utils.randomInt(100, 500)};
  }

  acceleration = 1.0f;
  deceleration = 0.95;
}

void logic() {
  input.processInput();

  if (input.keyPressed("return")) {
    selection += 1;
    if (selection == 7) selection = 0;
  }

  if (input.keyDown("left")) {
    x_velocity -= acceleration;
  }

  if (input.keyDown("right")) {
    x_velocity += acceleration;
  }

  if (input.keyDown("up")) {
    y_velocity -= acceleration;
  }

  if (input.keyDown("down")) {
    y_velocity += acceleration;
  }

  spots[selection].x += (int) x_velocity;
  spots[selection].y += (int) y_velocity;

  x_velocity *= deceleration;
  y_velocity *= deceleration;

  if (input.keyPressed("quit") > 0) {
    quit = true;
  }

  if (input.threeQuickKey("escape")) {
    quit = true;
  }
}

bool test_collision(int i, int j) {
  position l1a = {spots[0].x, spots[0].y + 180};
  position l1b = {spots[0].x + 180, spots[0].y};

  position l2a = {spots[1].x, spots[1].y + 180};
  position l2b = {spots[1].x + 180, spots[1].y};

  position l3a = {spots[2].x, spots[2].y};
  position l3b = {spots[2].x + 180, spots[2].y + 90};

  position b1 = spots[3];
  position b2 = spots[4];

  position c1 = spots[5];
  position c2 = spots[6];

  if (i == 0) {
    // Line 1
    if (j == 1) {
      // Line 1 and line 2
      return collisions.test(l1a, l1b, l2a, l2b);
    } else if (j == 2) {
      // Line 1 and line 3
      return collisions.test(l1a, l1b, l3a, l3b);
    } else if (j == 3) {
      // Line 1 and box 1
      return collisions.test(b1, box1, l1a, l1b);
    } else if (j == 4) {
      // Line 1 and box 2
      return collisions.test(b2, box2, l1a, l1b);
    } else if (j == 6) {
      // Line 1 and circle 1
      return collisions.test(c1, circle_1_radius, l1a, l1b);
    } else if (j == 6) {
      // Line 1 and circle 2
      return collisions.test(c2, circle_2_radius, l1a, l1b);
    }
  } else if (i == 1) {
    // Line 2
    if (j == 0) {
      // Line 2 and line 1
      return collisions.test(l2a, l2b, l1a, l1b);
    } else if (j == 2) {
      // Line 2 and line 3
      return collisions.test(l2a, l2b, l3a, l3b);
    } else if (j == 3) {
      // Line 2 and box 1
      return collisions.test(b1, box1, l2a, l2b);
    } else if (j == 4) {
      // Line 2 and box 2
      return collisions.test(b2, box2, l2a, l2b);
    } else if (j == 5) {
      // Line 2 and circle 1
      return collisions.test(c1, circle_1_radius, l2a, l2b);
    } else if (j == 6) {
      // Line 2 and circle 2
      return collisions.test(c2, circle_2_radius, l2a, l2b);
    }
  } else if (i == 2) {
    // Line 3
    if (j == 0) {
      // Line 3 and line 1
      return collisions.test(l3a, l3b, l1a, l1b);
    } else if (j == 1) {
      // Line 3 and line 2
      return collisions.test(l3a, l3b, l2a, l2b);
    } else if (j == 3) {
      // Line 3 and box 1
      return collisions.test(b1, box1, l3a, l3b);
    } else if (j == 4) {
      // Line 3 and box 2
      return collisions.test(b2, box2, l3a, l3b);
    } else if (j == 5) {
      // Line 3 and circle 1
      return collisions.test(c1, circle_1_radius, l3a, l3b);
    } else if (j == 6) {
      // Line 3 and circle 2
      return collisions.test(c2, circle_2_radius, l3a, l3b);
    }
  } else if (i == 3) {
    // Box 1
    if (j == 0) {
      // box 1 and line 1
      return collisions.test(b1, box1, l1a, l1b);
    } else if (j == 1) {
      // box 1 and line 2
      return collisions.test(b1, box1, l2a, l2b);
    } else if (j == 2) {
      // box 1 and line 3
      return collisions.test(b1, box1, l3a, l3b);
    } else if (j == 4) {
      // box 1 and box 2
      return collisions.test(b1, box1, b2, box2);
    } else if (j == 5) {
      // box 1 and circle 1
      return collisions.test(c1, circle_1_radius, b1, box1);
    } else if (j == 6) {
      // box 1 and circle 2
      return collisions.test(c2, circle_2_radius, b1, box1);
    }
  } else if (i == 4) {
    // Box 2
    if (j == 0) {
      // box 1 and line 1
      return collisions.test(spots[i], box2, l1a, l1b);
    } else if (j == 1) {
      // box 1 and line 2
      return collisions.test(spots[i], box2, l2a, l2b);
    } else if (j == 2) {
      // box 1 and line 3
      return collisions.test(spots[i], box2, l3a, l3b);
    } else if (j == 3) {
      // box 1 and box 2
      return collisions.test(b2, box2, b1, box1);
    } else if (j == 5) {
      // box 1 and circle 1
      return collisions.test(c1, circle_1_radius, b2, box2);
    } else if (j == 6) {
      // box 1 and circle 2
      return collisions.test(c2, circle_2_radius, b2, box2);
    }
  } else if (i == 5) {
    // Circle 1
    if (j == 0) {
      // circle 1 and line 1
      return collisions.test(c1, circle_1_radius, l1a, l1b);
    } else if (j == 1) {
      // circle 1 and line 2
      return collisions.test(c1, circle_1_radius, l2a, l2b);
    } else if (j == 2) {
      // circle 1 and line 3
      return collisions.test(c1, circle_1_radius, l3a, l3b);
    } else if (j == 3) {
      // circle 1 and box 1
      return collisions.test(c1, circle_1_radius, b1, box1);
    } else if (j == 4) {
      // circle 1 and box 2
      return collisions.test(c1, circle_1_radius, b2, box2);
    } else if (j == 6) {
      // circle 1 and circle 2
      return collisions.test(c1, circle_1_radius, c2, circle_2_radius);
    }
  } else if (i == 6) {
    // Circle 2
    if (j == 0) {
      // circle 1 and line 1
      return collisions.test(c2, circle_2_radius, l1a, l1b);
    } else if (j == 1) {
      // circle 1 and line 2
      return collisions.test(c2, circle_2_radius, l2a, l2b);
    } else if (j == 2) {
      // circle 1 and line 3
      return collisions.test(c2, circle_2_radius, l3a, l3b);
    } else if (j == 3) {
      // circle 1 and box 1
      return collisions.test(c2, circle_2_radius, b1, box1);
    } else if (j == 4) {
      // circle 1 and box 2
      return collisions.test(c2, circle_2_radius, b2, box2);
    } else if (j == 5) {
      // circle 1 and circle 2
      return collisions.test(c2, circle_2_radius, c1, circle_1_radius);
    }
  }

  return false;
}

void render() {
  // Clear the screen to a soft white color
  graphics.clearScreen("#DDDDDD");

  // Switch to 2D drawing mode
  graphics.draw2D();

  for (int i = 0; i < 7; i++) {
    string color = "black";
    if (i == selection) {
      color = "green";
    } else {
      if (test_collision(selection, i)) color = "green";
    }

    bool centered = false;
    if (i == 5 || i == 6) centered = true;
    graphics.drawImage(pictures[i] + color, spots[i].x, spots[i].y, centered, 0, 1);
  }

  // Put everything we've drawn on screen
  graphics.updateDisplay();
}

