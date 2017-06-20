//
//  delta anim.cpp
//  Game Engine
//
//  Created by Indi Kernick on 17/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "delta anim.hpp"

Time::DeltaAnim::DeltaAnim(uint64_t duration, EdgeMode edgeMode)
  : duration(duration), edgeMode(edgeMode) {}

void Time::DeltaAnim::update(uint64_t delta) {
  if (state == RUNNING) {
    if (dir == FORWARD) {
      progressTime += delta;
    } else {
      progressTime -= delta;
    }
    handleEdges();
  }
}

void Time::DeltaAnim::start() {
  assert(state == STOPPED);
  state = RUNNING;
}

void Time::DeltaAnim::stop() {
  assert(state == RUNNING);
  state = STOPPED;
  progressTime = 0;
  dir = FORWARD;
}

void Time::DeltaAnim::pause() {
  assert(state == RUNNING);
  state = PAUSED;
}

void Time::DeltaAnim::resume() {
  assert(state == PAUSED);
  state = RUNNING;
}

void Time::DeltaAnim::setDir(Dir newDir) {
  dir = newDir;
}

void Time::DeltaAnim::flipDir() {
  dir = static_cast<Dir>(1 - dir);
}

Time::DeltaAnim::Dir Time::DeltaAnim::getDir() {
  return dir;
}

void Time::DeltaAnim::setDuration(uint64_t newDuration) {
  duration = newDuration;
}

uint64_t Time::DeltaAnim::getDuration() {
  return duration;
}

void Time::DeltaAnim::setEdgeMode(EdgeMode newEdgeMode) {
  edgeMode = newEdgeMode;
}

Time::DeltaAnim::EdgeMode Time::DeltaAnim::getEdgeMode() {
  return edgeMode;
}

bool Time::DeltaAnim::isRunning() {
  return state == RUNNING;
}

bool Time::DeltaAnim::isStopped() {
  return state == STOPPED;
}

bool Time::DeltaAnim::isPaused() {
  return state == PAUSED;
}

void Time::DeltaAnim::setProgress(double newProgress) {
  progressTime = duration * newProgress;
}

double Time::DeltaAnim::getProgress() {
  return static_cast<double>(progressTime) / duration;
}

void Time::DeltaAnim::handleEdges() {
  const double progress = getProgress();
  if (0.0 <= progress && progress <= 1.0) {
    return;
  }
  switch (edgeMode) {
    case NOTHING:
      break;
    case REPEAT:
      setProgress(Math::norm(progress));
      break;
    case PAUSE:
      setProgress(Math::clamp(progress, 0.0, 1.0));
      pause();
      break;
    case STOP:
      stop();
      break;
    case CHANGE_DIR:
      if (static_cast<int>(progress) % 2) {
        flipDir();
      }
      setProgress(Math::normMirror(progress));
  }
}
