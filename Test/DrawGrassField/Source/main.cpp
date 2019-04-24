/*
  DrawGrassField
*/

#include "honey.h"

#include <string>

using namespace Honey;
using namespace std;

/*

Todo:

Draw a parabolic arc
- Make a math function to get a point on a parabolic arc given a start, end, control, and portion.
- Add opengl immediate mode (yes I know that's gone) rect drawing

*/

bool quit;

float a_x = 400, a_y = 400, b_x = 600, b_y = 300, c_x = 450, c_y = 200;

void initialize() {
  quit = false;

  graphics.addImages("Art/", {
    "marker"
  });
}

// Crap line
void line(float x1, float y1, float x2, float y2, float width) {
  float d = math_utils.distance(x1, y1, x2, y2);
  point v = {(x2 - x1) / d, (y2 - y1) / d, 0};
  point n = math_utils.rotateVector(v.x, v.y, 90);
  graphics.drawQuad(0, 0,
    x1 - width * n.x / 2.0, y1 - width * n.y / 2.0,
    x2 - width * n.x / 2.0, y2 - width * n.y / 2.0,
    x2 + width * n.x / 2.0, y2 + width * n.y / 2.0,
    x1 + width * n.x / 2.0, y1 + width * n.y / 2.0);
}

void render() {
  graphics.draw2D();
  graphics.clearScreen("#808080");
  graphics.setColor("#FFFFFF", 1.0);
  graphics.drawQuad(400, 400,
    0, 0,
    0, 200,
    100, 200,
    100, 0);

  graphics.setColor("#CCFFCC", 0.5);
  graphics.drawQuad(300, 500,
    0, 78,
    0, 100,
    200, 100,
    200, 78);

  graphics.setColor("#000000", 1.0);
  graphics.drawQuad(300, 450,
    0, 99,
    0, 100,
    200, 100,
    200, 99);

  graphics.setColor("#000000", 1.0);
  line(a_x, a_y, b_x, b_y, 3.0);
  graphics.setColor("#000000", 0.3);
  line(a_x, a_y, b_x, b_y, 4.0);

  float divisions = 20;
  float max_width = 20;
  for (int i = 0; i < divisions; i++) {
    point p1 = math_utils.parabolicArc(a_x, a_y, b_x, b_y, c_x, c_y, i / divisions);
    point p2 = math_utils.parabolicArc(a_x, a_y, b_x, b_y, c_x, c_y, (i+1) / divisions);
    float width = 0;
    if (i <= divisions / 2.0) {
      width = max_width / 2 + (max_width / 2) * (i / (divisions / 2.0));
    } else {
      width = (divisions - i) / (divisions / 2.0) * max_width;
    }
    graphics.setColor("#2b9a65", 1.0);
    line(p1.x, p1.y, p2.x, p2.y, width);
    graphics.setColor("#2b9a65", 0.3);
    line(p1.x, p1.y, p2.x, p2.y, width + 1);

    // graphics.setColor("#000000", 1.0);
    // line(p1.x, p1.y, p2.x, p2.y, 2.0);
    // graphics.setColor("#000000", 0.3);
    // line(p1.x, p1.y, p2.x, p2.y, 3.0);
  }

  graphics.setColor("#FFFFFF", 1.0);
  graphics.drawImage("marker", a_x, a_y, true, 0, 1);
  graphics.drawImage("marker", b_x, b_y, true, 0, 1);
  graphics.drawImage("marker", c_x, c_y, true, 0, 1);
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
  StartHoney("DrawGrassField");

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

