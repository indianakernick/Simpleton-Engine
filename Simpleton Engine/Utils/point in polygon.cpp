//
//  point in polygon.cpp
//  Game Engine
//
//  Created by Indi Kernick on 4/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "point in polygon.hpp"

//http://stackoverflow.com/a/2922778

bool Utils::pointInPolygon(const glm::vec2 test, const std::vector<glm::vec2> &vert) {
  return pointInPolygon(test, vert.size(), vert.data());
}

bool Utils::pointInPolygon(const glm::vec2 test, const size_t nvert, const glm::vec2 *vert) {
  bool c = false;
  for (size_t i = 0, j = nvert - 1; i < nvert; j = i++) {
    if ((vert[i].y > test.y != vert[j].y > test.y) &&
      (test.x < (vert[j].x - vert[i].x) * (test.y - vert[i].y) / (vert[j].y - vert[i].y) + vert[i].x))
      c = !c;
  }
  return c;
}
