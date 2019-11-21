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

Move the control points

*/

bool quit;

//unsigned char pixels[1280*720*4];

const Uint32 rmask = 0x00ff0000;
const Uint32 gmask = 0x0000ff00;
const Uint32 bmask = 0x000000ff;
const Uint32 amask = 0xff000000;

map<string, point> parabola = {{"a", {400, 400, 0}}, {"b", {500, 175, 0}}, {"c", {350, 250, 0}}};

string selected = "";

bool grass_configured;

void debug(int x) {
  printf("Here %d\n", x);
}

void initialize() {
  quit = false;

  graphics.addImages("Art/", {
    "marker"
  });

  effects.makeOscillation("c_oscillation", 50, 4);
  effects.makeOscillation("b_oscillation", 100, 4);
  effects.start("c_oscillation");
  effects.start("b_oscillation");
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
    x1 + width * n.x / 2.0, y1 + width * n.y / 2.0
  );
}

void render() {
  graphics.draw2D();
  graphics.clearScreen("#808080", 0.0f);

  graphics.setColor("#505050", 1.0);
  graphics.drawImage("marker", parabola["a"].x, parabola["a"].y, true, 0, 1);
  graphics.drawImage("marker", parabola["b"].x, parabola["b"].y, true, 0, 1);
  graphics.drawImage("marker", parabola["c"].x, parabola["c"].y, true, 0, 1);

  float divisions = 20;
  float max_width = 30;
  point last_left;
  point last_right;
  for (int i = 0; i < divisions; i++) {
    float width = 0;
    if (i <= divisions / 2.0) {
      width = max_width / 2 + (max_width / 2) * (i / (divisions / 2.0));
    } else {
      width = (divisions - i) / (divisions / 2.0) * max_width;
    }

    point p1 = math_utils.parabolicArc(parabola["a"].x, parabola["a"].y, parabola["b"].x, parabola["b"].y, parabola["c"].x, parabola["c"].y, i / divisions);
    point p2 = math_utils.parabolicArc(parabola["a"].x, parabola["a"].y, parabola["b"].x, parabola["b"].y, parabola["c"].x, parabola["c"].y, (i+1) / divisions);
    float d = math_utils.distance(p1.x, p1.y, p2.x, p2.y);
    point v = {(p2.x - p1.x) / d, (p2.y - p1.y) / d, 0};
    point n = math_utils.rotateVector(v.x, v.y, 90);
    point current_left = {p2.x - width * n.x / 2.0f, p2.y - width * n.y / 2.0f, 0};
    point current_right = {p2.x + width * n.x / 2.0f, p2.y + width * n.y / 2.0f, 0};
    if (i == 0) {
      last_left = {p1.x - width * n.x / 2.0f, p1.y - width * n.y / 2.0f, 0};
      last_right = {p1.x + width * n.x / 2.0f, p1.y + width * n.y / 2.0f, 0};
    }

    graphics.setColor("#2b9a65", 1.0);
    graphics.drawQuad(0, 0,
      last_left.x, last_left.y,
      current_left.x, current_left.y,
      current_right.x, current_right.y,
      last_right.x, last_right.y
    );

    graphics.setColor("#1b8a55", 0.3);
    line(p1.x, p1.y, p2.x, p2.y, 3.0);
    graphics.setColor("#1b8a55", 1.0);
    line(p1.x, p1.y, p2.x, p2.y, 2.0);
      
    last_left = current_left;
    last_right = current_right;
  }

  // glReadPixels(0, 0, 1280, 720, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  // int l = 4 * (math_utils.randomInt(200,400) * 1280 + math_utils.randomInt(400,500));
  // printf("%d %d %d %d\n", pixels[l], pixels[l+1], pixels[l+2], pixels[l+3]);
}

void logic() {
  if (input.threeQuickKey("escape")) {
    quit = true;
  }

  vector<mouseEvent> mouse_events = input.getMouseEvents();
  for (mouseEvent event : mouse_events) {
    if (event.event == "down") {
      printf("Down\n");
      for (string s : {"a", "b", "c"}) {
        if (math_utils.distance(event.x, event.y, parabola[s].x, parabola[s].y) <= 13) {
          selected = s;
          printf("Selected %s\n", s.c_str());
        }
      }
    } else if (event.event == "move") {
      if (selected != "") {
        printf("%d, %d\n", event.x, event.y);
        parabola[selected].x = event.x;
        parabola[selected].y = event.y;
      }
    } else if (event.event == "up") {
      selected = "";
    }
  }

  if (input.keyPressed("return") > 0) {
    int min_x = min(parabola["a"].x - 30, parabola["b"].x - 30);
    int max_x = min(parabola["a"].x + 30, parabola["b"].x + 30);
    int min_y = min(parabola["a"].y - 30, parabola["b"].y - 30);
    int max_y = min(parabola["a"].y + 30, parabola["b"].y + 30);
    debug(1);
    unsigned char* pixels = new unsigned char[(max_x - min_x) * (max_y - min_y) * 4];
    debug(2);
    glReadPixels(min_x, min_y, max_x, max_y, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    debug(3);
    SDL_Surface* grass_blade = SDL_CreateRGBSurface(0, max_x - min_x, max_y - min_y, 32, rmask, gmask, bmask, amask);
    debug(4);
    SDL_LockSurface(grass_blade);
    debug(5);
    SDL_UnlockSurface(grass_blade);
    debug(6);
    graphics.addImageFromSurface("grass_blade", grass_blade);
    debug(7);
    grass_configured = true;
    debug(8);
    delete [] pixels;
    debug(9);
    printf("Configured\n");
  }

  //parabola["b"].x = 400 + effects.oscillation("b_oscillation");
  //parabola["c"].x = 400 - effects.oscillation("c_oscillation");
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

