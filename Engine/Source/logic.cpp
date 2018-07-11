/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "logic.h"

namespace Honey {
  Logic* logic = new Logic();

  Logic::Logic() {
    time_markers = {};
    time_locks = {};
    transient_counters = {};
    transient_counter_values = {};
  }

  // Remove a label from all logic
  void Logic::remove(std::string label) {
    time_markers.erase(label);
    time_locks.erase(label);
    transient_counters.erase(label);
    transient_counter_values.erase(label);
  }

  // Marks down the time for a label
  void Logic::markTime(std::string label) {
    // Get the time in milliseconds since the world was created.
    time_markers[label] = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
  }

  // Tells the time in seconds since a label was marked
  float Logic::timeSince(std::string label) {
    // Check if the marker exists; if not, return 0.
    if (time_markers.count(label) == 0) {
      return 0;
    }
    // Get the time in milliseconds since the world was created.
    unsigned long current_time = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    // Subtract the old marked time, and divide by 1000 to get the time difference in seconds.
    float time_difference = (current_time - time_markers[label]) / 1000.0;
    return time_difference;
  }

  // Locks this label for this many seconds.
  void Logic::makeTimeLock(std::string label, float seconds) {
    // Don't make the time lock if it already exists
    if (time_locks.count(label) == 1) {
      printf("Warning: timeLock %s already exists.\n", label.c_str());
      return;
    }

    time_locks[label] = seconds;
    markTime(label);
  }

  // Tells you if this label is still locked.
  bool Logic::isTimeLocked(std::string label) {
    // If there is no lock, return false; it's not time locked!
    if (time_locks.count(label) == 0) {
      return false;
    }

    // The lock has expired! Remove it, and return false; it's no longer time locked!
    if (time_markers.count(label) == 1 && timeSince(label) > time_locks[label]) {
      remove(label);
      return false;
    }

    // Otherwise, it's locked. Return true.
    return true;
  }

  // Make a counter for this label that lasts for this many seconds.
  void Logic::makeTransientCounter(std::string label, float seconds) {
    // Don't make the time lock if it already exists and hasn't expired.
    if (transient_counters.count(label) == 1 && timeSince(label) < transient_counters[label]) {
      printf("Warning: transient counter %s already exists.\n", label.c_str());
      return;
    }

    transient_counters[label] = seconds;
    transient_counter_values[label] = 0;
    markTime(label);
  }

  // Increment a transient counter if it's still valid.
  void Logic::incrementTransientCounter(std::string label, int value) {
    // If the label doesn't exist, ignore and return.
    if (transient_counters.count(label) == 0) {
      return;
    }

    // If the label exists but it has expired, destroy it and recreate it.
    if (time_markers.count(label) == 1 && timeSince(label) > transient_counters[label]) {
      float seconds = transient_counters[label];
      remove(label);
      makeTransientCounter(label, seconds);
    }

    transient_counter_values[label] += value;
  }

  // Give the value of a transient counter, or 0 if it's no longer valid.
  int Logic::transientCounterValue(std::string label) {
    // If the label doesn't exist, ignore and return 0.
    if (transient_counters.count(label) == 0) {
      return 0;
    }

    // If the label exists but it has expired, destroy it and return 0.
    if (time_markers.count(label) == 1 && timeSince(label) > transient_counters[label]) {
      remove(label);
      return 0;
    }

    return transient_counter_values[label];
  }
}