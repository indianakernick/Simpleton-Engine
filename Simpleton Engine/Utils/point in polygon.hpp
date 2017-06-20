//
//  point in polygon.hpp
//  Game Engine
//
//  Created by Indi Kernick on 4/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_point_in_polygon_hpp
#define engine_utils_point_in_polygon_hpp

#include <glm/vec2.hpp>
#include <vector>

namespace Utils {
  ///Determine whether a point is within a polygon
  bool pointInPolygon(const glm::vec2, const std::vector<glm::vec2> &);
  ///Determine whether a point is within a polygon
bool pointInPolygon(const glm::vec2, const size_t, const glm::vec2 *);
}

#endif
