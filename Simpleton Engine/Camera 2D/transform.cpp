//
//  transform.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "transform.hpp"

#include "props.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

using namespace Cam2D;

namespace {
  glm::vec2 mulPos(const glm::mat3 &mat, const glm::vec2 pos) {
    return mat * glm::vec3(pos.x, pos.y, 1.0f);
  }
}

glm::mat3 Transform::toPixels() const {
  return toPixelsMat;
}

glm::mat3 Transform::toMeters() const {
  return toMetersMat;
}

bool Transform::visibleMeters(const AABB aabbMeters) const {
  return windowBounds.interceptsWith(aabbMeters);
}

void Transform::calculate(const Props props) {
  toPixelsMat = glm::translate(
    glm::scale(
      glm::translate(
        {},
        static_cast<glm::vec2>(props.windowSize) / 2.0f
      ),
      {props.ppm, -props.ppm}
    ),
    -props.center
  );
  toMetersMat = glm::inverse(toPixelsMat);
  windowBounds.setPoint(mulPos(toMetersMat, {0.0f, 0.0f}));
  windowBounds.extendToEnclose(mulPos(toMetersMat, props.windowSize));
}
