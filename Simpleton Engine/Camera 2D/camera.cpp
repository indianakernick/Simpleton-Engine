//
//  camera.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "camera.hpp"

using namespace Cam2D;

void Camera::update(const glm::ivec2 windowSize, const float delta) {
  props.windowSize = windowSize;
  
  const glm::vec2 posTarget = targetPos ? targetPos->calcTarget(props)
                                        : props.center;
  const glm::vec2 center = move ? move->calcCenter(props, posTarget, delta)
                                : posTarget;
  
  const float scaleTarget = targetScale ? targetScale->calcTarget(props)
                                        : props.ppm;
  const float ppm = zoom ? zoom->calcPPM(props, scaleTarget, delta)
                         : scaleTarget;
  
  props.center = center;
  props.ppm = ppm;
  
  transform.calculate(props);
}
