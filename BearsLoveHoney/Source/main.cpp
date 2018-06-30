#include <string>

#include "window.h"

int main(int argc, char* args[]) {
  printf("Honey Engine, reporting for duty!\n");

  Honey::Window* w = new Honey::Window("Honey Engine", 800, 600, true);

  SDL_Delay(1000);

  w->destroy();
}