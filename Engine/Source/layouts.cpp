/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "layouts.h"

namespace Honey {
  Layouts* layouts = new Layouts();

  Layouts::Layouts() {

  }

  void Layouts::makeRowLayout(std::string label, int x, int y, int x_margin) {
    layoutSettings s;
    s.x = x;
    s.y = y;
    s.x_margin = x_margin;
    settings[label] = s;
  }

  void Layouts::makeTileLayout(std::string label, int x, int y, int x_margin, int y_margin) {
    layoutSettings s;
    s.x = x;
    s.y = y;
    s.x_margin = x_margin;
    s.y_margin = y_margin;
    settings[label] = s;
  }

  void Layouts::makeTileWrapLayout(std::string label, int x, int y, int x_margin, int y_margin, int num_per_row) {
    layoutSettings s;
    s.x = x;
    s.y = y;
    s.x_margin = x_margin;
    s.y_margin = y_margin;
    s.num_per_row = num_per_row;
    settings[label] = s;
  }

  void Layouts::makeStaggerLayout(std::string label, int x, int y, int x_margin, int y_margin) {
    layoutSettings s;
    s.x = x;
    s.y = y;
    s.x_margin = x_margin;
    s.y_margin = y_margin;
    settings[label] = s;
  }

  void Layouts::makeStaggerWrapLayout(std::string label, int x, int y, int x_margin, int y_margin, int num_per_row) {
    layoutSettings s;
    s.x = x;
    s.y = y;
    s.x_margin = x_margin;
    s.y_margin = y_margin;
    s.num_per_row = num_per_row;
    settings[label] = s;
  }

  pair Layouts::row(std::string label, int value) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return empty_pair;
    }

    layoutSettings s = settings[label];
    
    pair p;
    p.y = s.y;
    p.x = s.x + value * s.x_margin;

    return p;
  }

  pair Layouts::column(std::string label, int value) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return empty_pair;
    }

    layoutSettings s = settings[label];
    
    pair p;
    p.y = s.y + value * s.y_margin;
    p.x = s.x;

    return p;
  }

  pair Layouts::tile(std::string label, int col_value, int row_value) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return empty_pair;
    }

    layoutSettings s = settings[label];

    pair p;
    p.y = s.y + col_value * s.y_margin;
    p.x = s.x + row_value * s.x_margin;

    return p;
  }

  pair Layouts::tileWrap(std::string label, int value) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return empty_pair;
    }

    layoutSettings s = settings[label];

    pair p;
    p.y = s.y + (value / s.num_per_row) * s.y_margin;
    p.x = s.x + (value % s.num_per_row) * s.x_margin;

    return p;
  }

  pair Layouts::stagger(std::string label, int col_value, int row_value) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return empty_pair;
    }

    layoutSettings s = settings[label];

    pair p;
    p.y = s.y + col_value * s.y_margin;
    p.x = s.x + row_value * s.x_margin;

    if (col_value % 2 == 1) {
      p.x += s.x_margin / 2;
    }

    return p;
  }

  pair Layouts::staggerWrap(std::string label, int value) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return empty_pair;
    }

    layoutSettings s = settings[label];

    // 6 stars, then 5 stars
    int modulus = 2 * s.num_per_row - 1;
    int big_quotient = value / modulus;
    int big_remainder = value % modulus;
    int little_quotient = big_remainder / s.num_per_row;
    int little_remainder = big_remainder % s.num_per_row;

    pair p;

    p.y = s.y + big_quotient * 2 * s.y_margin;
    p.x = s.x + (little_remainder % s.num_per_row) * s.x_margin;
    if (little_quotient == 1) {
      p.y += s.y_margin;
      p.x += s.x_margin / 2;
    }
    
    return p;
  }

  void Layouts::remove(std::string label) {

  }

}