//
//  zoom to fit.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "zoom to fit.hpp"

#include "props.hpp"

using namespace Cam2D;

ZoomToFit::ZoomToFit(const glm::vec2 box)
  : box(box) {}

void ZoomToFit::setSize(const glm::vec2 newBox) {
  box = newBox;
}

float ZoomToFit::calcTarget(const Props props) {
  const float boxAspect = box.x / box.y;
  
  if (props.aspect < boxAspect) {
    return 2.0f / box.x;
  } else {
    return 2.0f / box.y / props.aspect;
  }
}
