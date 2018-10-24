/*

  Honey
  Copyright 2018 - Matthew Carlin

*/

#include "collisions.h"

using namespace std;

namespace Honey {
  Collisions& Collisions::instance() {
    static Collisions collisions_instance;
    return collisions_instance;
  }

  // BOX BOX
  bool Collisions::test(position p1, box b1, position p2, box b2) {
    if (b1.left + p1.x > b2.right + p2.x) return false;
    if (b2.left + p2.x > b1.right + p1.x) return false;
    if (b1.top + p1.y > b2.bottom + p2.y) return false;
    if (b2.top + p2.y > b1.bottom + p1.y) return false;

    return true;
  }

  // BOX LINE
  bool Collisions::test(position p1, box b1, position p2, position p3) {
    if (p2.x >= p1.x + b1.left
      && p2.x <= p1.x + b1.right
      && p2.y >= p1.y + b1.top
      && p2.y <= p1.y + b1.bottom) return true;

    if (p3.x >= p1.x + b1.left
      && p3.x <= p1.x + b1.right
      && p3.y >= p1.y + b1.top
      && p3.y <= p1.y + b1.bottom) return true;

    position box_1[4] = {
      (position) {(int) b1.left + p1.x, (int) b1.top + p1.y},
      (position) {(int) b1.right + p1.x, (int) b1.top + p1.y},
      (position) {(int) b1.left + p1.x, (int) b1.bottom + p1.y},
      (position) {(int) b1.left + p1.x, (int) b1.top + p1.y}
    };
    position box_2[4] = {
      (position) {(int) b1.right + p1.x, (int) b1.top + p1.y},
      (position) {(int) b1.right + p1.x, (int) b1.bottom + p1.y},
      (position) {(int) b1.right + p1.x, (int) b1.bottom + p1.y},
      (position) {(int) b1.left + p1.x, (int) b1.bottom + p1.y}
    };
    for (int i = 0; i < 4; i++) {
      if (test(box_1[i], box_2[i], p2, p3)) return true;
    }

    return false;
  }

  // CIRCLE CIRCLE
  bool Collisions::test(position p1, float r1, position p2, float r2) {
    return math_utils.distance(p1, p2) < r1 + r2;
  }

  // CIRCLE BOX
  bool Collisions::test(position p1, float r1, position p2, box b2) {
    // https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
    // "Either the circle's centre lies inside the rectangle, or one of the edges
    // of the rectangle has a point in the circle."
    if (p1.x >= p2.x + b2.left
      && p1.x <= p2.x + b2.right
      && p1.y >= p2.y + b2.top
      && p1.y <= p2.y + b2.bottom) return true;

    position box_1[4] = {
      (position) {(int) b2.left + p2.x, (int) b2.top + p2.y},
      (position) {(int) b2.right + p2.x, (int) b2.top + p2.y},
      (position) {(int) b2.left + p2.x, (int) b2.bottom + p2.y},
      (position) {(int) b2.left + p2.x, (int) b2.top + p2.y}
    };
    position box_2[4] = {
      (position) {(int) b2.right + p2.x, (int) b2.top + p2.y},
      (position) {(int) b2.right + p2.x, (int) b2.bottom + p2.y},
      (position) {(int) b2.right + p2.x, (int) b2.bottom + p2.y},
      (position) {(int) b2.left + p2.x, (int) b2.bottom + p2.y}
    };
    for (int i = 0; i < 4; i++) {
      if (test(p1, r1, box_1[i], box_2[i])) return true;
    }

    return false;
  }

  // CIRCLE LINE
  bool Collisions::test(position p1, float r1, position p2, position p3) {
    if (math_utils.distance(p1, p2) <= r1) return true;
    if (math_utils.distance(p1, p3) <= r1) return true;
    position p4 = math_utils.closestPoint(p2, p3, p1);
    bool in_segment = true;
    if (p4.x < p2.x && p4.x < p3.x) in_segment = false;
    if (p4.x > p2.x && p4.x > p3.x) in_segment = false;
    if (p4.y < p2.y && p4.y < p3.y) in_segment = false;
    if (p4.y > p2.y && p4.y > p3.y) in_segment = false;
    if (!in_segment) return false;
    return (math_utils.distance(p4, p1) <= r1);
  }

  // LINE LINE
  bool Collisions::test(position p1, position p2, position p3, position p4) {
    // https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection

    if (abs((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x)) < 0.00001) {
      if (test(p1, 2.0f, p3, p4)) return true;
      if (test(p2, 2.0f, p3, p4)) return true;
      return false;
    }

    position p5 = {
      (int) ((p1.x * p2.y - p1.y * p2.x) * (p3.x - p4.x) - (p1.x - p2.x) * (p3.x * p4.y - p3.y * p4.x)) 
          / ((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x)),
      (int) ((p1.x * p2.y - p1.y * p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x * p4.y - p3.y * p4.x))
          / ((p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x))
    };

    bool in_segment = true;
    if (p5.x < p1.x && p5.x < p2.x) in_segment = false;
    if (p5.x > p1.x && p5.x > p2.x) in_segment = false;
    if (p5.y < p1.y && p5.y < p2.y) in_segment = false;
    if (p5.y > p1.y && p5.y > p2.y) in_segment = false;
    if (p5.x < p3.x && p5.x < p4.x) in_segment = false;
    if (p5.x > p3.x && p5.x > p4.x) in_segment = false;
    if (p5.y < p3.y && p5.y < p4.y) in_segment = false;
    if (p5.y > p3.y && p5.y > p4.y) in_segment = false;

    return in_segment;
  }

  Collisions::Collisions() {
  }

  Collisions::~Collisions() {
  }
}
