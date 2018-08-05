/*!
  @Honey
  @author Matthew Carlin
  Copyright 2018
*/

#pragma once

#include <chrono>
#include <string>
#include <stdlib.h>
#include <unordered_map>

using namespace std;

namespace Honey {
  /*!
    The Logic class provides game logic convenience functions.

    Different functions are all referenced by string labels; these labels are shared across functions.

    Primarily covered in blog posts:

    http://www.friendsonmountains.com/blog/2018/07/11/lets-make-honey-version-0-05-basic-controls
  */
  class Logic final {
   public:
    /*!
      Singleton instance getter.

      @return This'll get you the one instance of Logic you're supposed to have.
    */
    static Logic& instance();

    /*!
      Remove a label from this system.
      
      @param label Name of the label.
    */
    void remove(string label);

    /*!
      Check if a label exists.
      
      @param label Name of the label.
      @returns true if and only if the label exists in this system.
    */
    bool check(string label);

    /*!
      Mark the current time for this label.
      
      @param label Name of the label.
    */
    void markTime(string label);

    /*!
      Store a duration time in seconds for this label.
      
      @param label Name of the label.
      @param seconds Intended duration in seconds.
    */
    void markDuration(string label, float seconds);

    /*!
      Get the time in seconds since this label was marked.
      
      @param label Name of the label.
      @returns the time since this label was last marked.
    */
    float timeSince(string label);

    /*!
      Get the intended duration in seconds of this label.
      
      @param label Name of the label.
      @returns duration of the label.
    */
    float duration(string label);

    /*!
      Lock this label for this many seconds. Calls to isTimeLocked will now return true.
      
      @param label Name of the label.
      @param seconds the duration of the lock.
    */
    void makeTimeLock(string label, float seconds);

    /*!
      Check whether a label has been time locked.
      
      @param label Name of the label.
      @returns true if and only if the label has been locked using makeTimeLock, less than a certain number of seconds ago.
    */
    bool isTimeLocked(string label);

    /*!
      Make an integer counter for this label that lasts for this many seconds.
      
      @param label Name of the label.
      @param seconds the duration of the counter.
    */
    void makeTransientCounter(string label, float seconds);

    /*!
      If this label still has a counter associated with it, increment that counter.
      
      @param label Name of the label.
      @param value How much to add to the counter.
    */
    void incrementTransientCounter(string label, int value);

    /*!
      Get the value of a transient counter, or 0 if it's expired or invalid.
      
      @param label Name of the label.
      @returns the value of the counter.
    */
    int transientCounterValue(string label);

    /*!
      Get a random integer between low and high (inclusive, eg 3,5 will return any of 3, 4, and 5).
      
      @param low lowest value.
      @param high highest value.
      @returns a random number between the lowest and highest values.
    */
    int randomInt(int low, int high);

   private:
    // Hide constructor, destructor, copy constructor and assignment operator
    Logic();
    ~Logic();

    Logic(const Logic&) = delete;
    Logic& operator=(const Logic&) = delete;
    Logic(Logic&&) = delete;
    Logic& operator=(Logic&&) = delete;

    unordered_map<string, unsigned long> time_markers;
    unordered_map<string, float> duration_markers;

    unordered_map<string, int> transient_counter_values;
  };

  extern Logic& logic;
}
