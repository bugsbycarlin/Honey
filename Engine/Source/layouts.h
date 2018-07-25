/*

  Honey
  Copyright 2018 - Matthew Carlin

  Layouts class has convenience methods for creating simple visual layouts.
*/

#ifndef HONEY_LAYOUTS_H_
#define HONEY_LAYOUTS_H_

// Standard libraries
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <unordered_map>

// Honey includes
#include "primitives.h"

namespace Honey {
  struct layoutSettings {
    int x;
    int y;
    int x_margin;
    int y_margin;
    int num_per_row;
  };

  class Layouts {
   public:

    // Types of layouts:
    //
    // Row layout: Looks like a 1d array of items in a single row.
    // Access: value
    // 1  2  3  4  5  6  7  8  9  10
    //
    //
    // Column layout: Looks like a 1d array of items in a single column.
    // Access: value
    // 1
    // 2
    // 3
    // 4
    //
    //
    // Tile layout: Looks like a 2d array of items in a grid.
    // Access: col_value, row_value
    // 1,1  1,2  1,3  1,4  1,5 ...
    // 2,1  2,2  2,3  2,4  2,5 ...
    // 3,1  3,2  3,3  3,4  3,5 ...
    // 
    //
    // TileWrap layout: a set of items that wrap around based on num_per_row
    // Access: value
    // If num_per_row is 4:
    // 1  2  3  4
    // 5  6  7  8
    // 9  10 11 12
    // 13 14 15 16
    //
    //
    // Stagger layout: Looks like the stars in an american flag, with every
    // other row indented by half margin.
    // Access: col_value, row_value
    // 1,1  1,2  1,3  1,4  1,5 ...
    //    2,1  2,2  2,3  2,4  2,5 ...
    // 3,1  3,2  3,3  3,4  3,5 ...
    //    4,1  4,2  4,3  4,4  4,5
    //
    //
    // StaggerWrap layout: items wrap based on num_per_row, but every other
    // row is indented and has one less item.
    // Access: value
    // If num_per_row is 5:
    // 1   2   3   4   5
    //   6   7   8   9
    // 10  11  12  13  14
    //   15  16  17  18
    //

    std::unordered_map<std::string, layoutSettings> settings;

    Layouts();

    void makeRowLayout(std::string label, int x, int y, int x_margin);
    void makeColumnLayout(std::string label, int x, int y, int x_margin);
    void makeTileLayout(std::string label, int x, int y, int x_margin, int y_margin);
    void makeTileWrapLayout(std::string label, int x, int y, int x_margin, int y_margin, int num_per_row);
    void makeStaggerLayout(std::string label, int x, int y, int x_margin, int y_margin);
    void makeStaggerWrapLayout(std::string label, int x, int y, int x_margin, int y_margin, int num_per_row);

    pair row(std::string label, int value);
    pair column(std::string label, int value);
    pair tile(std::string label, int col_value, int row_value);
    pair tileWrap(std::string label, int value);
    pair stagger(std::string label, int col_value, int row_value);
    pair staggerWrap(std::string label, int value);

    void remove(std::string label);
  };

  extern Layouts* layouts;
}
#endif
