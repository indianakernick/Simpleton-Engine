//
//  frame anim.cpp
//  Game Engine
//
//  Created by Indi Kernick on 21/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "frame anim.hpp"

Time::FrameAnim::FrameAnim(uint8_t animFPS, uint16_t animLength, uint8_t displayFPS)
  : animFPS(animFPS), animLength(animLength), displayFPS(displayFPS) {}

void Time::FrameAnim::update() {
  frame++;
}

uint16_t Time::FrameAnim::getFrame() {
  return animFPS * std::fmod(static_cast<double>(frame) / displayFPS,
                             static_cast<double>(animLength) / animFPS);
}
