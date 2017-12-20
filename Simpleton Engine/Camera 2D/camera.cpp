//
//  camera.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "camera.hpp"

using namespace Cam2D;

Camera::Camera(const float scale) {
  props.pos = {0.0f, 0.0f};
  props.scale = scale;
}

Camera::Camera(const glm::vec2 pos, const float scale) {
  props.pos = pos;
  props.scale = scale;
}

void Camera::setPos(const glm::vec2 pos) {
  props.pos = pos;
}

void Camera::setZoom(const float scale) {
  props.scale = scale;
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

void Camera::update(const float aspect, const float delta) {
  props.aspect = aspect;
  
  const glm::vec2 pos = animate(targetPos, animatePos, props, delta);
  const float scale = animate(targetZoom, animateZoom, props, delta);
  
  props.pos = pos;
  props.scale = scale;
  
  transform.calculate(props);
}
