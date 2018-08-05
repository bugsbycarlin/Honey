/*!
  @Honey
  @author Matthew Carlin
  Copyright 2018

  Primitives has some useful structs and typedefs that get usefully used all over this place.
*/

#pragma once

#include <string>
#include <unordered_map>
#include <boost/variant.hpp>

using namespace std;

namespace Honey {
  /*!
    position holds integer screen coordinates x and y.
  */
  struct position {
    int x;
    int y;
  };

  const position origin = {0, 0};

  /*!
    point holds float space coordinates x, y, and z.
  */
  struct point {
    float x;
    float y;
    float z;
  };

  /*!
    floatColor holds float color values r, g, and b which should be between 0 and 1.
  */
  struct floatColor {
    float r;
    float g;
    float b;
  };

  /*!
    intColor holds int color values r, g, and b which should be between 0 and 255.
  */
  struct intColor {
    int r;
    int g;
    int b;
  };
}
