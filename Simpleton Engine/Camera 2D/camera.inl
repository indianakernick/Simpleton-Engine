//
//  camera.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

inline Cam2D::Camera::Camera(const float scale) {
  props.pos = {0.0f, 0.0f};
  props.scale = scale;
}

inline Cam2D::Camera::Camera(const glm::vec2 pos, const float scale) {
  props.pos = pos;
  props.scale = scale;
}

inline void Cam2D::Camera::setPos(const glm::vec2 pos) {
  props.pos = pos;
}

inline void Cam2D::Camera::setZoom(const float scale) {
  props.scale = scale;
}

inline void Cam2D::Camera::setAngle(const float angle) {
  props.angle = angle;
}

namespace Cam2D::detail {
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

inline void Cam2D::Camera::update(const float aspect, const float delta) {
  props.aspect = aspect;
  
  const glm::vec2 pos = detail::animate(targetPos, animatePos, props, delta);
  const float scale = detail::animate(targetZoom, animateZoom, props, delta);
  const float angle = detail::animate(targetAngle, animateAngle, props, delta);
  
  props.pos = pos;
  props.scale = scale;
  props.angle = angle;
  
  transform.calculate(props);
}
