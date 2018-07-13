/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "effects.h"

namespace Honey {
  Effects* effects = new Effects();

  Effects::Effects() {
    srand (time(NULL));

    tween_starts = {};
    tween_ends = {};
    shake_widths = {};
  }

  void Effects::makeTween(std::string label, float start_value, float end_value, float seconds) {
    logic->markDuration(label, seconds);
    logic->markTime(label);
    tween_starts[label] = start_value;
    tween_ends[label] = end_value;
  }

  float Effects::tween(std::string label, int style) {
    // If there's no tween, return 0
    if (logic->time_markers.count(label) == 0) {
      return 0;
    }

    // If the tween is expired, destroy it and return 0
    if (logic->timeSince(label) > logic->duration(label)) {
      logic->remove(label);
      tween_starts.erase(label);
      tween_ends.erase(label);
      return 0;
    }

    // The time fraction is from 0 (no time has elapsed) to 1 (all the time has elapsed)
    float time_fraction = logic->timeSince(label) / logic->duration(label);
    float param;
    float space_fraction;

    switch (style) {
      case LINEAR: // constant speed
        return (1 - time_fraction) * tween_starts[label] + time_fraction * tween_ends[label];

      case SIGMOID: // slow at the start, fast in the middle, slow at the end
        param = (time_fraction * 2.0) - 1.0;
        space_fraction = 1.0 / (1 + exp(-1 * sigmoid_steepness * param));
        return (1 - space_fraction) * tween_starts[label] + space_fraction * tween_ends[label];

      case CUBIC: // fast at the start, slow in the middle, fast at the end
        param = (time_fraction * 4.0) - 2.0;
        space_fraction = (pow(param, 3) / 8.0 + 1.0) / 2.0;
        return (1 - space_fraction) * tween_starts[label] + space_fraction * tween_ends[label];

      case RAMPDOWN: // fast at the start, medium in the middle, slow at the end
        param = time_fraction - 1.0;
        space_fraction = pow(param, 3) + 1.0;
        return (1 - space_fraction) * tween_starts[label] + space_fraction * tween_ends[label];

      case RAMPUP: // slow at the start, medium in the middle, fast at the end
        space_fraction = pow(time_fraction, 3);
        return (1 - space_fraction) * tween_starts[label] + space_fraction * tween_ends[label];

      case SINEWAVE: // oscillates from start value at t=0 to end value at t=0.5 to start value at t=1
        space_fraction = sin(M_PI * time_fraction);
        return (1 - space_fraction) * tween_starts[label] + space_fraction * tween_ends[label];
    }

    printf("Warning: unknown tween style.\n");
    return 0;
  }

  void Effects::makeShake(std::string label, int shake_width, float seconds) {
    logic->markDuration(label, seconds);
    logic->markTime(label);
    shake_widths[label] = shake_width;
  }

  float Effects::shake(std::string label) {
    // If there's no label, return 0
    if (logic->time_markers.count(label) == 0) {
      return 0;
    }

    // If the label is expired, destroy it and return 0
    if (logic->timeSince(label) > logic->duration(label)) {
      logic->remove(label);
      shake_widths.erase(label);
      return 0;
    }

    // Return a random number between -width/2 and width/2
    return rand() % (int) shake_widths[label] - shake_widths[label] / 2.0;
  }
}