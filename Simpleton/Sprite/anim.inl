//
//  anim.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 10/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include <cassert>

inline Sprite::Anim::Anim()
  : sprite_{NULL_SPRITE}, frames_{0}, frame_{0}, delay_{0} {}

inline Sprite::Anim::Anim(
  const ID sprite,
  const ID frames,
  const bool enabled
) : sprite_{sprite}, frames_{frames}, frame_{enabled ? 0 : STOPPED}, delay_{1} {
  assert(sprite_ != NULL_SPRITE);
  assert(frames_ != 0);
}

inline Sprite::Anim::Anim(
  const ID sprite,
  const ID frames,
  const ID delay,
  const bool enabled
) : sprite_{sprite}, frames_{frames}, frame_{enabled ? 0 : STOPPED}, delay_{delay} {
  assert(sprite_ != NULL_SPRITE);
  assert(frames_ != 0);
  assert(delay_ != 0);
}

inline void Sprite::Anim::incr() {
  if (frame_ != STOPPED) {
    assert(frame_ < frames_ * delay_);
    ++frame_;
  }
}

inline void Sprite::Anim::incrRepeat() {
  if (frame_ != STOPPED) {
    assert(frame_ < frames_ * delay_);
    frame_ = (frame_ + 1) % (frames_ * delay_);
  }
}

inline bool Sprite::Anim::incrStop() {
  if (frame_ != STOPPED) {
    assert(frame_ < frames_ * delay_);
    ++frame_;
    const bool end = (frame_ == frames_ * delay_);
    frame_ -= end;
    return end;
  } else {
    return false;
  }
}

inline void Sprite::Anim::start() {
  frame_ = 0;
}

inline void Sprite::Anim::stop() {
  frame_ = STOPPED;
}

inline bool Sprite::Anim::enabled() const {
  return frame_ != STOPPED;
}

inline bool Sprite::Anim::disabled() const {
  return frame_ == STOPPED;
}

inline void Sprite::Anim::delay(const ID delay) {
  assert(delay != 0);
  if (delay_ != delay) {
    if (frame_ != STOPPED) {
      frame_ /= delay_;
      frame_ *= delay;
    }
    delay_ = delay;
  }
}

inline void Sprite::Anim::noDelay() {
  delay(1);
}

inline void Sprite::Anim::maxDelay() {
  delay(NULL_SPRITE / frames_);
}

inline void Sprite::Anim::speed(const double speed) {
  assert(speed > 0.0);
  delay(1.0 / speed + 0.5);
}

inline Sprite::ID Sprite::Anim::firstSprite() const {
  return sprite_;
}

inline Sprite::ID Sprite::Anim::sprite() const {
  assert(sprite_ != NULL_SPRITE);
  assert((frame_ & MASK) / delay_ < frames_);
  return sprite_ + (frame_ & MASK) / delay_;
}

inline Sprite::ID Sprite::Anim::sprite(const ID group) const {
  return sprite() + frames_ * group;
}

template <typename T>
Sprite::ID Sprite::Anim::sprite(const T group) const {
  return sprite(static_cast<ID>(group));
}

inline Sprite::ID Sprite::Anim::frame() const {
  return frame_ / delay_;
}

inline Sprite::ID Sprite::Anim::frameOrZero() const {
  return (frame_ & MASK) / delay_;
}

inline float Sprite::Anim::progress() const {
  assert(frames_ != 0);
  if (frames_ == 1) {
    return 0.0f;
  } else {
    return static_cast<float>(frame_ & MASK) / ((frames_ - 1) * delay_);
  }
}

inline bool Sprite::Anim::firstFrame() const {
  return frame_ == 0;
}

inline bool Sprite::Anim::lastFrame() const {
  return frame_ == (frames_ - 1) * delay_;
}
