/*

  Honey
  Copyright 2018 - Matthew Carlin

  all_the_honey includes every Honey class in one project.
*/

#include "window.h"
#include "logic.h"
#include "hotconfig.h"
#include "input.h"
#include "graphics.h"
#include "layouts.h"
#include "textbox.h"
#include "sound.h"
#include "effects.h"

using namespace Honey;

void QuitHoney() {
  delete effects;
  delete sound;
  delete layouts;
  delete graphics;
  delete input;
  delete hot_config;
  delete logic;
  delete window;
}