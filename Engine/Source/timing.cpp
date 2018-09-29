/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "timing.h"

using namespace std;

namespace Honey {
  Timing& Timing::instance() {
    static Timing time_instance;
    return time_instance;
  }

  Timing::Timing() {
    time_markers = {};
    duration_markers = {};
    transient_counter_values = {};
    sequence_timings = {};
  }

  void Timing::remove(string label) {
    time_markers.erase(label);
    duration_markers.erase(label);
    transient_counter_values.erase(label);
  }

  bool Timing::check(string label) {
    return time_markers.count(label) == 1;
  }

  void Timing::mark(string label) {
    // Get the time in milliseconds since the world was created.
    time_markers[label] = chrono::system_clock::now().time_since_epoch() / chrono::milliseconds(1);
  }

  void Timing::setDuration(string label, float seconds) {
    duration_markers[label] = seconds;
  }

  float Timing::since(string label) {
    // Check if the marker exists; if not, return 0.
    if (time_markers.count(label) == 0) {
      return 0;
    }
    // Get the time in milliseconds since the world was created.
    unsigned long current_time = chrono::system_clock::now().time_since_epoch() / chrono::milliseconds(1);
    // Subtract the old marked time, and divide by 1000 to get the time difference in seconds.
    float time_difference = (current_time - time_markers[label]) / 1000.0;
    return time_difference;
  }

  float Timing::duration(string label) {
    if (!check(label)) {
      return 0;
    }
    return duration_markers[label];
  }

  void Timing::lock(string label, float seconds) {
    // Don't make the time lock if it already exists
    if (check(label)) {
      printf("Warning: label %s already exists.\n", label.c_str());
      return;
    }

    setDuration(label, seconds);
    mark(label);
  }

  bool Timing::locked(string label) {
    // If there is no lock, return false; it's not time locked!
    if (!check(label)) {
      return false;
    }

    // The lock has expired! Remove it, and return false; it's no longer time locked!
    if (check(label) && since(label) > duration_markers[label]) {
      remove(label);
      return false;
    }

    // Otherwise, it's locked. Return true.
    return true;
  }

  void Timing::makeTransientCounter(string label, float seconds) {
    // Don't make the time lock if it already exists and hasn't expired.
    if (check(label) && since(label) < duration_markers[label]) {
      printf("Warning: transient counter %s already exists.\n", label.c_str());
      return;
    }

    duration_markers[label] = seconds;
    transient_counter_values[label] = 0;
    mark(label);
  }

  void Timing::incrementTransientCounter(string label, int value) {
    // If the label doesn't exist, ignore and return.
    if (!check(label)) {
      return;
    }

    // If the label exists but it has expired, destroy it and recreate it.
    if (check(label) && since(label) > duration_markers[label]) {
      float seconds = duration_markers[label];
      remove(label);
      makeTransientCounter(label, seconds);
    }

    transient_counter_values[label] += value;
  }

  int Timing::transientCounterValue(string label) {
    // If the label doesn't exist, ignore and return 0.
    if (!check(label)) {
      return 0;
    }

    // If the label exists but it has expired, destroy it and return 0.
    if (check(label) && since(label) > duration_markers[label]) {
      remove(label);
      return 0;
    }

    return transient_counter_values[label];
  }

  void Timing::makeSequence(string label, vector<float> sequence_timing) {
    // If the label already exists, it will be overwritten.
    sequence_timings[label] = sequence_timing;
    sequence_counters[label] = -1;
    mark(label);
  }

  int Timing::sequenceValue(string label) {
    // If the label doesn't exist, ignore and return 0.
    if (!check(label) || sequence_timings.count(label) != 1) {
      return 0;
    }

    float elapsed_sequence_time = since(label);
    float candidate_duration = 0;
    int candidate_value = 0;
    for (float duration : sequence_timings[label]) {
      candidate_duration += duration;
      if (elapsed_sequence_time < candidate_duration) {
        return candidate_value;
      }
      candidate_value += 1;
    }

    return candidate_value;
  }

  void Timing::makeSequenceWithFunction(string label, vector<float> sequence_timing, function<void(int, float)> action) {
    makeSequence(label, sequence_timing);
    sequence_counters[label] = -1;
    sequence_actions[label] = action;
  }

  void Timing::doSequence(string label) {
    // If the label doesn't exist, ignore.
    if (!check(label) || sequence_timings.count(label) != 1 || sequence_actions.count(label) != 1) {
      return;
    }

    int current_value = sequenceValue(label);
    if (current_value > sequence_counters[label]) {
      sequence_counters[label] = current_value;
      sequence_actions[label](sequence_counters[label], sequence_timings[label][sequence_counters[label]]);
    }
  }

  Timing::~Timing() {
    time_markers.clear();
    duration_markers.clear();
    transient_counter_values.clear();
  }
}
