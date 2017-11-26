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

void Camera::setZoom(const float ppm) {
  props.ppm = ppm;
}

namespace {
  template <PropID PROP>
  auto animate(
    std::unique_ptr<Target<PROP>> &target,
    std::unique_ptr<Animate<PROP>> &animate,
    const Props &props,
    const float delta
  ) {
    const auto targetProp = target ? target->calcTarget(props) : getProp<PROP>(props);
    return animate ? animate->calculate(props, targetProp, delta) : targetProp;
  }
}

void Camera::update(const glm::ivec2 windowSize, const float delta) {
  props.windowSize = windowSize;
  
  const glm::vec2 center = animate(targetPos, animatePos, props, delta);
  const float ppm = animate(targetZoom, animateZoom, props, delta);
  
  props.center = center;
  props.ppm = ppm;
  
  transform.calculate(props);
}
