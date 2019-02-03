/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "layouts.h"

using namespace std;

namespace Honey {
  Layouts& Layouts::instance() {
    static Layouts layouts_instance;
    return layouts_instance;
  }

  Layouts::Layouts() {
  }

  void Layouts::makeRowLayout(string label, int x, int y, int x_margin) {
    LayoutSettings s;
    s.x = x;
    s.y = y;
    s.x_margin = x_margin;
    settings[label] = s;
  }

  void Layouts::makeColumnLayout(string label, int x, int y, int y_margin) {
    LayoutSettings s;
    s.x = x;
    s.y = y;
    s.y_margin = y_margin;
    settings[label] = s;
  }

  void Layouts::makeTileLayout(string label, int x, int y, int x_margin, int y_margin) {
    LayoutSettings s;
    s.x = x;
    s.y = y;
    s.x_margin = x_margin;
    s.y_margin = y_margin;
    settings[label] = s;
  }

  void Layouts::makeTileWrapLayout(string label, int x, int y, int x_margin, int y_margin, int num_per_row) {
    LayoutSettings s;
    s.x = x;
    s.y = y;
    s.x_margin = x_margin;
    s.y_margin = y_margin;
    s.num_per_row = num_per_row;
    settings[label] = s;
  }

  void Layouts::makeStaggerLayout(string label, int x, int y, int x_margin, int y_margin) {
    LayoutSettings s;
    s.x = x;
    s.y = y;
    s.x_margin = x_margin;
    s.y_margin = y_margin;
    settings[label] = s;
  }

  void Layouts::makeStaggerWrapLayout(string label, int x, int y, int x_margin, int y_margin, int num_per_row) {
    LayoutSettings s;
    s.x = x;
    s.y = y;
    s.x_margin = x_margin;
    s.y_margin = y_margin;
    s.num_per_row = num_per_row;
    settings[label] = s;
  }

  position Layouts::row(string label, int element_number) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return origin;
    }

    LayoutSettings s = settings[label];

    position p;
    p.y = s.y;
    p.x = s.x + element_number * s.x_margin;

    return p;
  }

  position Layouts::column(string label, int element_number) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return origin;
    }

    LayoutSettings s = settings[label];

    position p;
    p.y = s.y + element_number * s.y_margin;
    p.x = s.x;

    return p;
  }

  position Layouts::tile(string label, int column_number, int row_number) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return origin;
    }

    LayoutSettings s = settings[label];

    position p;
    p.y = s.y + row_number * s.y_margin;
    p.x = s.x + column_number * s.x_margin;

    return p;
  }

  position Layouts::tileWrap(string label, int element_number) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return origin;
    }

    LayoutSettings s = settings[label];

    position p;
    p.y = s.y + (element_number / s.num_per_row) * s.y_margin;
    p.x = s.x + (element_number % s.num_per_row) * s.x_margin;

    return p;
  }

  position Layouts::stagger(string label, int column_number, int row_number) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return origin;
    }

    LayoutSettings s = settings[label];

    position p;
    p.y = s.y + row_number * s.y_margin;
    p.x = s.x + column_number * s.x_margin;

    if (column_number % 2 == 1) {
      p.x += s.x_margin / 2;
    }

    return p;
  }

  position Layouts::staggerWrap(string label, int element_number) {
    if (settings.count(label) == 0) {
      printf("Can't find layout %s\n", label.c_str());
      return origin;
    }

    LayoutSettings s = settings[label];

    // 6 stars, then 5 stars
    int modulus = 2 * s.num_per_row - 1;
    int big_quotient = element_number / modulus;
    int big_remainder = element_number % modulus;
    int little_quotient = big_remainder / s.num_per_row;
    int little_remainder = big_remainder % s.num_per_row;

    position p;

    p.y = s.y + big_quotient * 2 * s.y_margin;
    p.x = s.x + (little_remainder % s.num_per_row) * s.x_margin;
    if (little_quotient == 1) {
      p.y += s.y_margin;
      p.x += s.x_margin / 2;
    }

    return p;
  }

  void Layouts::remove(string label) {
    settings.erase(label);
  }

  Layouts::~Layouts() {
    settings.clear();
  }
}
