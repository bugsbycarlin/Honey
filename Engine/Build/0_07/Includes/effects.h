/*

  Honey
  Copyright 2018 - Matthew Carlin

  Effects class provides convenience methods to create visual effects.
*/

#ifndef HONEY_EFFECTS_H_
#define HONEY_EFFECTS_H_

#include <chrono>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <unordered_map>

#include "logic.h"

namespace Honey {
  class Effects {
   public:

      Effects();

      enum STYLES {
        LINEAR,
        SIGMOID,
        CUBIC,
        RAMPDOWN,
        RAMPUP,
        SINEWAVE
      };

      const float sigmoid_steepness = 5;

      std::unordered_map<std::string, float> tween_starts;
      std::unordered_map<std::string, float> tween_ends;
      std::unordered_map<std::string, float> shake_widths;

      void makeTween(std::string label, float start_value, float end_value, float seconds);
      float tween(std::string label, int style);

      void makeShake(std::string label, int shake_width, float seconds);
      float shake(std::string label);
  };

  extern Effects* effects;
}
#endif
