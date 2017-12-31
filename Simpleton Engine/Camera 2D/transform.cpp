//
//  transform.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "transform.hpp"

#include "props.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

using namespace Cam2D;

void Transform::setOrigin(const Origin newOrigin) {
  origin = newOrigin;
}

void Transform::setInvertX(const bool newInvertX) {
  invertX = newInvertX;
}

void Transform::setInvertY(const bool newInvertY) {
  invertY = newInvertY;
}

glm::mat3 Transform::toPixels() const {
  return toPixelsMat;
}

glm::mat3 Transform::toMeters() const {
  return toMetersMat;
}

void Transform::calculate(const Props props) {
  const glm::mat3 originMat = glm::translate({}, calcOriginPos());
  const glm::mat3 zoomMat = glm::scale({}, glm::vec2(props.scale));
  const glm::mat3 invertMat = glm::scale({}, calcInvertedScale());
  const glm::mat3 aspectMat = glm::scale({}, glm::vec2(1.0f, props.aspect));
  const glm::mat3 posMat = glm::translate({}, -props.pos);

  toPixelsMat = originMat * zoomMat * invertMat * aspectMat * posMat;
  toMetersMat = glm::inverse(toPixelsMat);
}

glm::vec2 Transform::calcOriginPos() const {
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

glm::vec2 Transform::calcInvertedScale() const {
  return {
    invertX ? -1.0f : 1.0f,
    invertY ? -1.0f : 1.0f
  };
}
