//
//  transform.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "props.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

inline void Cam2D::Transform::setOrigin(const Origin newOrigin) {
  origin = newOrigin;
}

inline void Cam2D::Transform::setInvertX(const bool newInvertX) {
  invertX = newInvertX;
}

inline void Cam2D::Transform::setInvertY(const bool newInvertY) {
  invertY = newInvertY;
}

inline glm::mat3 Cam2D::Transform::toPixels() const {
  return toPixelsMat;
}

inline glm::mat3 Cam2D::Transform::toMeters() const {
  return toMetersMat;
}

inline void Cam2D::Transform::calculate(const Props props) {
  const glm::mat3 originMat = glm::translate({}, calcOriginPos());
  const glm::mat3 zoomMat = glm::scale({}, glm::vec2(props.scale));
  const glm::mat3 invertMat = glm::scale({}, calcInvertedScale());
  const glm::mat3 aspectMat = glm::scale({}, glm::vec2(1.0f, props.aspect));
  const glm::mat3 posMat = glm::translate({}, -props.pos);

  toPixelsMat = originMat * zoomMat * invertMat * aspectMat * posMat;
  toMetersMat = glm::inverse(toPixelsMat);
}

inline glm::vec2 Cam2D::Transform::calcOriginPos() const {
  switch (origin) {
    case Origin::TOP_LEFT:
      return {-1.0f, 1.0f};
    case Origin::TOP_MID:
      return {0.0f, 1.0f};
    case Origin::TOP_RIGHT:
      return {1.0f, 1.0f};
    case Origin::MID_RIGHT:
      return {1.0f, 0.0f};
    case Origin::BOTTOM_RIGHT:
      return {1.0f, -1.0f};
    case Origin::BOTTOM_MID:
      return {0.0f, -1.0f};
    case Origin::BOTTOM_LEFT:
      return {-1.0f, -1.0f};
    case Origin::MID_LEFT:
      return {-1.0f, 0.0f};
    case Origin::CENTER:
      return {0.0f, 0.0f};
  }
}

inline glm::vec2 Cam2D::Transform::calcInvertedScale() const {
  return {
    invertX ? -1.0f : 1.0f,
    invertY ? -1.0f : 1.0f
  };
}
