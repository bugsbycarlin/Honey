/*
  Collisions Test Program
  Matthew Carlin
  Copyright 2018
*/


#include <array>
#include <assert.h>
#include <string>

#include "honey.h"

using namespace Honey;
using namespace std;

void initialize();

void test();
void testMathUtils();

// Functions
int main(int argc, char* args[]) {
  initialize();
  test();
}


void initialize() {
}

void test () {
  testMathUtils();
}

void testMathUtils() {
  // Test vector rotation
  float x = 54;
  float y = 0;

  for (int i = -9; i <= 9; i++) {
    point p = math_utils.rotateVector(x, y, 10 * i);
    printf("Vector rotated by %d is %0.3f, %0.3f\n", 10 * i, p.x, p.y);
  }

  point p;

  p = math_utils.rotateVector(54, 0, 10);
  assert(abs(p.x - 53.180) < 0.01);
  assert(abs(p.y - 9.377) < 0.01);

  p = math_utils.rotateVector(54, 0, -10);
  assert(abs(p.x - 53.180) < 0.01);
  assert(abs(p.y + 9.377) < 0.01);

  p = math_utils.rotateVector(54, 0, 90);
  assert(abs(p.x) < 0.01);
  assert(abs(p.y - 54) < 0.01);

  p = math_utils.rotateVector(54, 0, -90);
  assert(abs(p.x) < 0.01);
  assert(abs(p.y + 54) < 0.01);
}