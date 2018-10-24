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

  position MathUtilities::closestPoint(position p1, position p2, position p3) {
    // TODO: stop doing this in ints. Change position to doubles maybe.
    if (abs(p2.x - p1.x) < 0.000001) {
      return (position) {(int) p1.x, (int) p3.y};
    } else if (abs(p2.y - p1.y) < 0.000001) {
      return (position) {(int) p3.x, (int) p1.y};
    } else {
      double slope = (double) (p2.y - p1.y) / (double) (p2.x - p1.x);
      double perpendicular_slope = -1.0 / slope;

      // Line 1 equation is p1.y = slope * p1.x + b1
      // So,
      double b1 = p1.y - slope * p1.x;
      // Line 1 equation is y = slope * x + p1.y - slope * p1.x
      // Similarly
      // Line 2 equation is y = perpendicular_slope * x + p3.y - perpendicular_slope * p3.x
      // To find the intersection, set the equations equal and solve for x:
      // slope * x + p1.y - slope * p1.x = perpendicular_slope * x + p3.y - perpendicular_slope * p3.x
      // So,
      double x = (p3.y - perpendicular_slope * p3.x - p1.y + slope * p1.x) / (slope - perpendicular_slope);
      // And plug back in to the first equation to get,
      double y = slope * x + p1.y - slope * p1.x;
      return (position) {(int) x, (int) y};
    }
  }

  MathUtilities::~MathUtilities() {
  }
}
