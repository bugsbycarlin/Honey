/*

  Honey
  Copyright 2018 - Matthew Carlin

  Primitives has some useful structs and typedefs that get usefully used all over this place.
*/

#pragma once

#include <string>
#include <unordered_map>
#include <boost/variant.hpp>

using namespace std;

namespace Honey {
  struct position {
    int x;
    int y;
  };

  const position origin = {0, 0};

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

  // This is a cheap way to build settings dictionaries like
  // settings s = {
  // {"type", "tween"},
  // {"style", "sigmoid"},
  // {"label", "bear_movement"},
  // {"active", 0},
  // {"start", 1},
  // {"end", 0},
  // {"duration", 1.5}}
  typedef boost::variant<int, double, string> primitive;
  typedef unordered_map<string, primitive> settings;
}
