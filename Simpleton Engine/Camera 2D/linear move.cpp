//
//  linear move.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "linear move.hpp"

#include "props.hpp"
#include <glm/gtx/norm.hpp>

using namespace Cam2D;

LinearMove::LinearMove(const float speed)
  : speed(speed) {}

glm::vec2 LinearMove::calcCenter(
  const Props props,
  const glm::vec2 target,
  const float delta
) {
  const glm::vec2 toTarget = target - props.center;
  const float targetDist = glm::length(toTarget);
  const float moveDist = speed * delta;
  if (targetDist <= moveDist) {
    return target;
  } else {
    return props.center + toTarget * moveDist;
  }
}
