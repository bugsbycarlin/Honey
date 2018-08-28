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
    The Timing class provides time logic convenience functions.

    Different functions are all referenced by string labels; these labels are shared across functions.

    Primarily covered in blog posts:

    http://www.friendsonmountains.com/blog/2018/07/11/lets-make-honey-version-0-05-basic-controls
    http://www.friendsonmountains.com/blog/2018/08/28/lets-make-honey-version-0-15-attaching-effects
  */
  class Timing final {
   public:
    /*!
      Singleton instance getter.

      @return This'll get you the one instance of Timing you're supposed to have.
    */
    static Timing& instance();

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
    void mark(string label);

    /*!
      Store a duration time in seconds for this label.
      
      @param label Name of the label.
      @param seconds Intended duration in seconds.
    */
    void setDuration(string label, float seconds);

    /*!
      Get the time in seconds since this label was marked.
      
      @param label Name of the label.
      @returns the time since this label was last marked.
    */
    float since(string label);

    /*!
      Get the intended duration in seconds of this label.
      
      @param label Name of the label.
      @returns duration of the label.
    */
    float duration(string label);

    /*!
      Lock this label for this many seconds. Calls to locked will now return true.
      
      @param label Name of the label.
      @param seconds the duration of the lock.
    */
    void lock(string label, float seconds);

    /*!
      Check whether a label has been time locked.
      
      @param label Name of the label.
      @returns true if and only if the label has been locked using makeTimeLock, less than a certain number of seconds ago.
    */
    bool locked(string label);

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

   private:
    // Hide constructor, destructor, copy constructor and assignment operator
    Timing();
    ~Timing();

    Timing(const Timing&) = delete;
    Timing& operator=(const Timing&) = delete;
    Timing(Timing&&) = delete;
    Timing& operator=(Timing&&) = delete;

    unordered_map<string, unsigned long> time_markers;
    unordered_map<string, float> duration_markers;

    unordered_map<string, int> transient_counter_values;
  };

  extern Timing& timing;
}
