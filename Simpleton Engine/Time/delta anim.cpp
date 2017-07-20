//
//  delta anim.cpp
//  Game Engine
//
//  Created by Indi Kernick on 17/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "delta anim.hpp"

Time::DeltaAnim::DeltaAnim(const uint64_t duration)
  : duration(duration) {}

Time::DeltaAnim::DeltaAnim(const uint64_t duration, const EdgeMode edgeMode, const Dir dir)
  : duration(duration), edgeMode(edgeMode), dir(dir) {}

void Time::DeltaAnim::advance(const uint64_t delta) {
  if (state == State::RUNNING) {
    if (dir == Dir::FORWARD) {
      progressTime += delta;
    } else {
      progressTime -= delta;
    }
    handleEdges();
  }
}

void Time::DeltaAnim::start() {
  assert(state == State::STOPPED);
  state = State::RUNNING;
}

void Time::DeltaAnim::stop() {
  assert(state == State::RUNNING);
  state = State::STOPPED;
  progressTime = 0;
  dir = Dir::FORWARD;
}

void Time::DeltaAnim::pause() {
  assert(state == State::RUNNING);
  state = State::PAUSED;
}

void Time::DeltaAnim::resume() {
  assert(state == State::PAUSED);
  state = State::RUNNING;
}

void Time::DeltaAnim::setDir(Dir newDir) {
  dir = newDir;
}

void Time::DeltaAnim::flipDir() {
  dir = static_cast<Dir>(1 - dir);
}

Time::DeltaAnim::Dir Time::DeltaAnim::getDir() const {
  return dir;
}

void Time::DeltaAnim::setDuration(const uint64_t newDuration) {
  duration = newDuration;
}

uint64_t Time::DeltaAnim::getDuration() const {
  return duration;
}

void Time::DeltaAnim::setEdgeMode(const EdgeMode newEdgeMode) {
  edgeMode = newEdgeMode;
}

Time::DeltaAnim::EdgeMode Time::DeltaAnim::getEdgeMode() const {
  return edgeMode;
}

bool Time::DeltaAnim::isRunning() const {
  return state == State::RUNNING;
}

bool Time::DeltaAnim::isStopped() const {
  return state == State::STOPPED;
}

bool Time::DeltaAnim::isPaused() const {
  return state == State::PAUSED;
}

void Time::DeltaAnim::setProgress(double newProgress) {
  progressTime = duration * newProgress;
}

double Time::DeltaAnim::getProgress() const {
  return static_cast<double>(progressTime) / duration;
}

void Time::DeltaAnim::handleEdges() {
  const double progress = getProgress();
  if (0.0 <= progress && progress <= 1.0) {
    return;
  }
  switch (edgeMode) {
    case EdgeMode::NOTHING:
      break;
    case EdgeMode::REPEAT:
      setProgress(Math::norm(progress));
      break;
    case EdgeMode::PAUSE:
      setProgress(Math::clamp(progress, 0.0, 1.0));
      pause();
      break;
    case EdgeMode::STOP:
      stop();
      break;
    case EdgeMode::CHANGE_DIR:
      if (static_cast<int>(progress) % 2) {
        flipDir();
      }
      setProgress(Math::normMirror(progress));
  }
}
