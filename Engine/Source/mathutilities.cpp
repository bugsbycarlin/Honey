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

  float MathUtilities::distance(position p1, position p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
  }

  MathUtilities::~MathUtilities() {
  }
}
