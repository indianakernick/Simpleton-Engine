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

template <Cam2D::PropID ...TARGET_PROPS, Cam2D::PropID ...ANIM_PROPS>
void Cam2D::Camera::update(
  const Params params,
  Target<TARGET_PROPS> &... targets,
  Animate<ANIM_PROPS> &... anims
) {
  //Is this code too flexible?

  Props targetProps = props;
  ((getProp<TARGET_PROPS>(targetProps) = targets.calcTarget(props, params)), ...);
  ((getProp<ANIM_PROPS>(targetProps) = anims.calculate(props, params, getProp<ANIM_PROPS>(targetProps))), ...);
  props = targetProps;
  transform.calculate(props, params);
}

template <Cam2D::PropID ...TARGET_PROPS>
void Cam2D::Camera::update(
  const Params params,
  Target<TARGET_PROPS> &... targets
) {
  Props targetProps = props;
  ((getProp<TARGET_PROPS>(targetProps) = targets.calcTarget(props, params)), ...);
  props = targetProps;
  transform.calculate(props, params);
}
