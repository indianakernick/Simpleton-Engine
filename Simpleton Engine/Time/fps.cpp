//
//  fps.cpp
//  Game Engine
//
//  Created by Indi Kernick on 16/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "fps.hpp"

Time::FPS::FPS(uint8_t avgSeconds)
  : totalFrames(60 * avgSeconds) {
  assert(avgSeconds);
  while (avgSeconds--) {
    frames.push(60);
  }
  frames.push(0);
}

void Time::FPS::init() {
  uint8_t avgSeconds = static_cast<uint8_t>(frames.size() - 1);
  totalFrames = 60 * avgSeconds;
  while (avgSeconds--) {
    frames.pop();
    frames.push(60);
  }
  frames.pop();
  frames.push(0);
  startTime = getI<std::chrono::nanoseconds>();
}

void Time::FPS::frame() {
  uint64_t now = getI<std::chrono::nanoseconds>();
  while (now - startTime >= MATH_SI(ONE, NANO)) {
    startTime += MATH_SI(ONE, NANO);
    totalFrames -= frames.front();
    frames.pop();
    totalFrames += frames.back();
    frames.push(0);
  }
  frames.back()++;
}

double Time::FPS::get() {
  return static_cast<double>(totalFrames) / (frames.size() - 1);
}
