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

template <Cam2D::PropID... PROPS>
Cam2D::Props Cam2D::Camera::calcTargetProps(
  const Params params,
  Cam2D::Target<PROPS> &... targets
) {
  Props targetProps = props;
  ((getProp<PROPS>(targetProps) = targets.calcTarget(props, params)), ...);
  return targetProps;
}

template <Cam2D::PropID... PROPS>
void Cam2D::Camera::animateProps(
  Cam2D::Props targetProps,
  const Cam2D::Params params,
  Cam2D::Animate<PROPS> &... anims
) {
  ((getProp<PROPS>(targetProps) = anims.calculate(props, params, getProp<PROPS>(targetProps))), ...);
  props = targetProps;
  transform.calculate(props, params);
}
