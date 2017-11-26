//
//  camera.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "camera.hpp"

using namespace Cam2D;

Camera::Camera(const float ppm) {
  props.center = {0.0f, 0.0f};
  props.ppm = ppm;
}

Camera::Camera(const glm::vec2 center, const float ppm) {
  props.center = center;
  props.ppm = ppm;
}

void Camera::setPos(const glm::vec2 center) {
  props.center = center;
}

void Camera::setScale(const float ppm) {
  props.ppm = ppm;
}

void Camera::update(const glm::ivec2 windowSize, const float delta) {
  props.windowSize = windowSize;
  
  const glm::vec2 posTarget = targetPos ? targetPos->calcTarget(props)
                                        : props.center;
  const glm::vec2 center = animatePos ? animatePos->calculate(props, posTarget, delta)
                                      : posTarget;
  
  const float scaleTarget = targetScale ? targetScale->calcTarget(props)
                                        : props.ppm;
  const float ppm = animateZoom ? animateZoom->calculate(props, scaleTarget, delta)
                                : scaleTarget;
  
  props.center = center;
  props.ppm = ppm;
  
  transform.calculate(props);
}
