/*

  Honey
  Copyright 2018 - Matthew Carlin

  Logic class provides game logic convenience functions.
*/

#ifndef HONEY_LOGIC_H_
#define HONEY_LOGIC_H_

#include <chrono>
#include <string>
#include <stdlib.h>
#include <unordered_map>

namespace Honey {
  class Logic {
   public:
      std::unordered_map<std::string, unsigned long> time_markers;
      std::unordered_map<std::string, float> duration_markers;

      std::unordered_map<std::string, int> transient_counter_values;

      Logic();

      // Remove a label from all logic
      void remove(std::string label);

      // Marks down the time for a label
      void markTime(std::string label);

      // Marks the intended duration of a label
      void markDuration(std::string label, float seconds);

      // Tells the time in seconds since a label was marked
      float timeSince(std::string label);

      // Remind me of the intended duration of the label
      float duration(std::string label);

      // Locks this label for this many seconds.
      void makeTimeLock(std::string label, float seconds);

      // Tells you if this label is still locked.
      bool isTimeLocked(std::string label);

      // Make a counter for this label that lasts for this many seconds.
      void makeTransientCounter(std::string label, float seconds);

      // Increment a transient counter if it's still valid.
      void incrementTransientCounter(std::string label, int value);

      // Give the value of a transient counter, or 0 if it's no longer valid.
      int transientCounterValue(std::string label);

      // Get a random integer between start and end
      int randomInt(int start, int end);
  };

  extern Logic* logic;
}
#endif
