//
//  transform.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "transform.hpp"

#include "props.hpp"
#include "matrix mul.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>

using namespace Cam2D;

void Transform::setOrigin(const Origin newOrigin) {
  posOrigin = newOrigin;
  zoomOrigin = newOrigin;
}

void Transform::setPosOrigin(const Origin newOrigin) {
  posOrigin = newOrigin;
}

void Transform::setPosOriginSize(const glm::vec2 newOriginSize) {
  posOriginSize = newOriginSize;
}

void Transform::setZoomOrigin(const Origin newOrigin) {
  zoomOrigin = newOrigin;
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

glm::vec2 Transform::toPixels(const glm::vec2 pos) const {
  return mulPos(toPixelsMat, pos);
}

glm::vec2 Transform::toMeters(const glm::vec2 pos) const {
  return mulPos(toMetersMat, pos);
}

bool Transform::visibleMeters(const AABB aabbMeters) const {
  return windowBounds.interceptsWith(aabbMeters);
}

namespace {
  glm::vec2 getOriginPos(const Origin origin) {
    switch (origin) {
      case Origin::TOP_LEFT:
        return {0.0f, 0.0f};
      case Origin::TOP_RIGHT:
        return {1.0f, 0.0f};
      case Origin::BOTTOM_RIGHT:
        return {1.0f, 1.0f};
      case Origin::BOTTOM_LEFT:
        return {0.0f, 1.0f};
      case Origin::CENTER:
        return {0.5f, 0.5f};
    }
  }
  
  glm::mat3 getOriginTransform(const glm::vec2 windowSize, const Origin origin) {
    return glm::translate({}, windowSize * getOriginPos(origin));
  }
  
  glm::vec2 getInvertedScale(const bool invertX, const bool invertY) {
    return {
      invertX ? -1.0f : 1.0f,
      invertY ? -1.0f : 1.0f
    };
  }
}

void Transform::calculate(const Props props) {
  const glm::vec2 posOriginPos = getOriginPos(posOrigin) - getOriginPos(zoomOrigin);
  const glm::mat3 posOriginMat = glm::translate({}, posOriginSize * posOriginPos);
  const glm::mat3 zoomOriginMat = getOriginTransform(props.windowSize, zoomOrigin);
  const glm::mat3 zoomMat = glm::scale({}, glm::vec2(props.ppm));
  const glm::mat3 posMat = glm::translate({}, -props.center);
  const glm::mat3 invertMat = glm::scale({}, getInvertedScale(invertX, invertY));

  toPixelsMat = zoomOriginMat * zoomMat * posOriginMat * invertMat * posMat;
 
  toMetersMat = glm::inverse(toPixelsMat);
  windowBounds.setPoint(mulPos(toMetersMat, {0.0f, 0.0f}));
  windowBounds.extendToEnclose(mulPos(toMetersMat, props.windowSize));
}
