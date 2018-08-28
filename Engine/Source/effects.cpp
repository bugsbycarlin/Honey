/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "effects.h"

using namespace std;

namespace Honey {
  Effects& Effects::instance() {
    static Effects effects_instance;
    return effects_instance;
  }

  Effects::Effects() {
    tween_starts = {};
    tween_ends = {};
    sizes = {};
  }

  bool Effects::check(string label) {
    return timing.check(label);
  }

  void Effects::removeAllEffects() {
    for (pair<string, float> item : tween_starts) {
      timing.remove(item.first);
    }
    tween_starts.clear();
    tween_ends.clear();
    sizes.clear();
  }

  void Effects::makeTween(string label, float start_value, float end_value, float duration_in_seconds) {
    timing.setDuration(label, duration_in_seconds);
    timing.mark(label);
    tween_starts[label] = start_value;
    tween_ends[label] = end_value;
  }

  float Effects::tween(string label, int style) {
    // If there's no tween, return 0
    if (!timing.check(label)) {
      return 0;
    }

    // If the tween is expired, return the end value
    if (timing.since(label) > timing.duration(label)) {
      return tween_ends[label];
    }

    // The time fraction is from 0 (no time has elapsed) to 1 (all the time has elapsed)
    float time_fraction = timing.since(label) / timing.duration(label);
    float param;
    float space_fraction;

    switch (style) {
      // constant speed
      case LINEAR:
        return (1 - time_fraction) * tween_starts[label] + time_fraction * tween_ends[label];

      // slow at the start, fast in the middle, slow at the end
      case SIGMOID:
        param = (time_fraction * 2.0) - 1.0;
        space_fraction = 1.0 / (1 + exp(-1 * sigmoid_steepness * param));
        return (1 - space_fraction) * tween_starts[label] + space_fraction * tween_ends[label];

      // fast at the start, slow in the middle, fast at the end
      case CUBIC:
        param = (time_fraction * 4.0) - 2.0;
        space_fraction = (pow(param, 3) / 8.0 + 1.0) / 2.0;
        return (1 - space_fraction) * tween_starts[label] + space_fraction * tween_ends[label];

      // fast at the start, medium in the middle, slow at the end
      case RAMPDOWN:
        param = time_fraction - 1.0;
        space_fraction = pow(param, 3) + 1.0;
        return (1 - space_fraction) * tween_starts[label] + space_fraction * tween_ends[label];

      // slow at the start, medium in the middle, fast at the end
      case RAMPUP:
        space_fraction = pow(time_fraction, 3);
        return (1 - space_fraction) * tween_starts[label] + space_fraction * tween_ends[label];

      // oscillates from start value at t=0 to end value at t=0.5 to start value at t=1
      case SINEWAVE:
        space_fraction = sin(M_PI * time_fraction);
        return (1 - space_fraction) * tween_starts[label] + space_fraction * tween_ends[label];
    }

    printf("Warning: unknown tween style.\n");
    return 0;
  }

  void Effects::makeShake(string label, int shake_size, float duration_in_seconds) {
    timing.setDuration(label, duration_in_seconds);
    timing.mark(label);
    sizes[label] = shake_size;
  }

  float Effects::shake(string label) {
    // If there's no label, return 0
    if (!timing.check(label)) {
      return 0;
    }

    // If the label is expired, remove it and return 0
    if (timing.since(label) > timing.duration(label)) {
      timing.remove(label);
      sizes.erase(label);
      return 0;
    }

    // Return a random number between -width/2 and width/2
    return rand() % (int) sizes[label] - sizes[label] / 2.0;
  }

  void Effects::makeOscillation(string label, float oscillation_size, float period_in_seconds) {
    timing.setDuration(label, period_in_seconds);
    timing.mark(label);
    sizes[label] = oscillation_size;
  }

  float Effects::oscillation(string label) {
    // If there's no label, return 0
    if (!timing.check(label)) {
      return 0;
    }

    // We *don't* check if the oscillation has expired. Oscillations are infinite, until they're manually deleted.

    // We're misusing duration for period
    float time_fraction = timing.since(label) / timing.duration(label);
    float space_fraction = sin(2 * M_PI * time_fraction);
    return (2 * space_fraction - 1) * sizes[label];
  }

  Effects::~Effects() {
    removeAllEffects();
  }
}
