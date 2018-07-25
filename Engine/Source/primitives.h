/*

  Honey
  Copyright 2018 - Matthew Carlin

  Primitives has some useful structs that get usefully used all over this place.
*/

#ifndef HONEY_PRIMITIVES_H_
#define HONEY_PRIMITIVES_H_

#include <string>
#include <stdlib.h>

namespace Honey {
  struct pair {
    int x;
    int y;
  };

  const pair empty_pair = {0, 0};

  struct point {
    float x;
    float y;
    float z;
  };

  // These values are assumed to range from 0 to 1
  struct floatColor {
    float r;
    float g;
    float b;
  };

  // These values are assumed to range from 0 to 255
  struct intColor {
    int r;
    int g;
    int b;
  };
}
#endif
