/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "mathutilities.h"

using namespace std;

namespace Honey {
  MathUtilities& MathUtilities::instance() {
    static MathUtilities math_instance;
    return math_instance;
  }

  MathUtilities::MathUtilities() {
    srand(time(NULL));
  }

  int MathUtilities::randomInt(int low, int high) {
    return rand() % (int) (high - low) + low;
  }

  MathUtilities::~MathUtilities() {
  }
}
