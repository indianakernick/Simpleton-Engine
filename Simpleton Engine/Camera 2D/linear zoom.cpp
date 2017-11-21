//
//  linear zoom.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "linear zoom.hpp"

#include <cmath>
#include "props.hpp"
#include "../Math/scale.hpp"

using namespace Cam2D;

LinearZoom::LinearZoom(const float speed)
  : speed(speed) {}

float LinearZoom::calcPPM(
  const Props props,
  const float target,
  const float delta
) {
  const float toTarget = target - props.ppm;
  const float targetDist = std::abs(toTarget);
  const float zoomDist = speed * delta;
  if (targetDist <= zoomDist) {
    return target;
  } else {
    return props.ppm + Math::sign(toTarget) * zoomDist;
  }
}
