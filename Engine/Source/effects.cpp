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
    state = {};
    start_value = {};
    end_value = {};
    size = {};
  }

  void Effects::start(string label) {
    if (state.count(label) != 1) {
      printf("Error: failed to find effect %s while starting.\n", label.c_str());
      return;
    }
    state[label] = BUSY;
    timing.mark(label);
  }

  void Effects::stop(string label) {
    if (state.count(label) != 1) {
      printf("Error: failed to find effect %s while stopping.\n", label.c_str());
      return;
    }
    state[label] = FINISHED;
  }

  bool Effects::waiting(string label) {
    return state.count(label) == 1 && state[label] == WAITING;
  }

  bool Effects::busy(string label) {
    return state.count(label) == 1 && state[label] == BUSY;
  }

  bool Effects::finished(string label) {
    return state.count(label) == 1 && state[label] == FINISHED;
  }

  void Effects::removeAllEffects() {
    for (pair<string, float> item : start_value) {
      timing.remove(item.first);
    }
    state.clear();
    start_value.clear();
    end_value.clear();
    size.clear();
  }

  void Effects::makeTween(string label, float start_value, float end_value, float duration_in_seconds) {
    timing.setDuration(label, duration_in_seconds);
    this->start_value[label] = start_value;
    this->end_value[label] = end_value;
    state[label] = WAITING;
  }

  float Effects::tween(string label, int style) {
    // If there's no tween timing, return 0
    if (!timing.check(label)) {
      return 0;
    }

    if (state[label] == WAITING) {
      return start_value[label];
    } else if (state[label] == FINISHED) {
      return end_value[label];
    }

    // The time fraction is from 0 (no time has elapsed) to 1 (all the time has elapsed)
    float time_fraction = timing.since(label) / timing.duration(label);
    float param;
    float space_fraction;

    // If the tween is expired, stop the tween and set the fractional value to the end time
    if (timing.since(label) > timing.duration(label)) {
      stop(label);
      time_fraction = 1.0;
    }

    switch (style) {
      // constant speed
      case LINEAR:
        return (1 - time_fraction) * start_value[label] + time_fraction * end_value[label];

      // slow at the start, fast in the middle, slow at the end
      case SIGMOID:
        param = (time_fraction * 2.0) - 1.0;
        space_fraction = 1.0 / (1 + exp(-1 * sigmoid_steepness * param));
        return (1 - space_fraction) * start_value[label] + space_fraction * end_value[label];

      // fast at the start, slow in the middle, fast at the end
      case CUBIC:
        param = (time_fraction * 4.0) - 2.0;
        space_fraction = (pow(param, 3) / 8.0 + 1.0) / 2.0;
        return (1 - space_fraction) * start_value[label] + space_fraction * end_value[label];

      // fast at the start, medium in the middle, slow at the end
      case RAMPDOWN:
        param = time_fraction - 1.0;
        space_fraction = pow(param, 3) + 1.0;
        return (1 - space_fraction) * start_value[label] + space_fraction * end_value[label];

      // slow at the start, medium in the middle, fast at the end
      case RAMPUP:
        space_fraction = pow(time_fraction, 3);
        return (1 - space_fraction) * start_value[label] + space_fraction * end_value[label];

      // oscillates from start value at t=0 to end value at t=0.5 to start value at t=1
      case SINEWAVE:
        space_fraction = sin(M_PI * time_fraction);
        return (1 - space_fraction) * start_value[label] + space_fraction * end_value[label];
    }

    printf("Warning: unknown tween style.\n");
    return 0;
  }

  void Effects::makeShake(string label, int shake_size, float duration_in_seconds) {
    timing.setDuration(label, duration_in_seconds);
    size[label] = shake_size;
    state[label] = WAITING;
  }

  float Effects::shake(string label) {
    // If there's no shake timing, return 0
    if (!timing.check(label)) {
      return 0;
    }

    // If the shake isn't busy shaking, return 0
    if (state[label] != BUSY) {
      return 0;
    }

    // If the label is expired, remove it and return 0
    if (timing.since(label) > timing.duration(label)) {
      timing.remove(label);
      size.erase(label);
      return 0;
    }

    // Return a random number between -width/2 and width/2
    return rand() % (int) size[label] - size[label] / 2.0;
  }

  void Effects::makeOscillation(string label, float oscillation_size, float period_in_seconds) {
    timing.setDuration(label, period_in_seconds);
    size[label] = oscillation_size;
    state[label] = WAITING;
  }

  float Effects::oscillation(string label) {
    // If there's no oscillation timing, return 0
    if (!timing.check(label)) {
      return 0;
    }

    // If the oscillation isn't busy, return 0
    if (state[label] != BUSY) {
      return 0;
    }

    // We *don't* check if the oscillation has expired. Oscillations are infinite, until they're turned off or deleted.

    // We're misusing duration for period
    float time_fraction = timing.since(label) / timing.duration(label);
    float space_fraction = sin(2 * M_PI * time_fraction);
    return space_fraction * size[label];
  }

  Effects::~Effects() {
    removeAllEffects();
  }
}
