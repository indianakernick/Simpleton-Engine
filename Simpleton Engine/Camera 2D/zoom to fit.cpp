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
  const float windowAspect = props.windowSize.x / props.windowSize.y;
  const float boxAspect = box.x / box.y;
  
  if (windowAspect < boxAspect) {
    return props.windowSize.x / box.x;
  } else {
    return props.windowSize.y / box.y;
  }
}
