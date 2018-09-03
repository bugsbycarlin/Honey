/*!
  @Honey
  @author Matthew Carlin
  Copyright 2018
*/

#pragma once

#include <string>
#include <stdlib.h>

using namespace std;

namespace Honey {
  /*!
    The MathUtilities class provides math utility functions.

    http://www.friendsonmountains.com/blog/2018/08/28/lets-make-honey-version-0-15-attaching-effects
  */
  class MathUtilities final {
   public:
    /*!
      Singleton instance getter.

      @return This'll get you the one instance of Time you're supposed to have.
    */
    static MathUtilities& instance();

    /*!
      Get a random integer between low and high (inclusive, eg 3,5 will return any of 3, 4, and 5).
      
      @param low lowest value.
      @param high highest value.
      @returns a random number between the lowest and highest values.
    */
    int randomInt(int low, int high);

   private:
    // Hide constructor, destructor, copy constructor and assignment operator
    MathUtilities();
    ~MathUtilities();

    MathUtilities(const MathUtilities&) = delete;
    MathUtilities& operator=(const MathUtilities&) = delete;
    MathUtilities(MathUtilities&&) = delete;
    MathUtilities& operator=(MathUtilities&&) = delete;
  };

  extern MathUtilities& math_utilities;
}
