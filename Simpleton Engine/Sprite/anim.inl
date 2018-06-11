//
//  anim.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 10/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include <cassert>

inline Sprite::Anim::Anim()
  : sprite_{NULL_SPRITE}, frames_{0}, frame_{0} {}

inline Sprite::Anim::Anim(const ID sprite, const ID frames)
  : sprite_{sprite}, frames_{frames}, frame_{0} {
  assert(frames_ != 0);
}

inline void Sprite::Anim::incr() {
  assert(frame_ < frames_);
  ++frame_;
}

inline void Sprite::Anim::incrRepeat() {
  assert(frame_ < frames_);
  frame_ = (frame_ + 1) % frames_;
}

inline bool Sprite::Anim::incrStop() {
  assert(frame_ < frames_);
  ++frame_;
  const bool end = (frame_ == frames_);
  frame_ -= end;
  return end;
}

inline Sprite::ID Sprite::Anim::firstSprite() const {
  return sprite_;
}

inline Sprite::ID Sprite::Anim::sprite() const {
  assert(frame_ < frames_);
  assert(sprite_ != NULL_SPRITE);
  return sprite_ + frame_;
}

inline Sprite::ID Sprite::Anim::sprite(const ID group) const {
  return sprite() + frames_ * group;
}

template <typename T>
Sprite::ID Sprite::Anim::sprite(const T group) const {
  return sprite(static_cast<ID>(group));
}

inline Sprite::DelayAnim::DelayAnim()
  : sprite_{NULL_SPRITE}, frames_{0}, delay_{0}, frame_{0} {}

inline Sprite::DelayAnim::DelayAnim(
  const ID sprite,
  const ID frames,
  const ID delay
) : sprite_{sprite}, frames_{frames}, delay_{delay}, frame_{0} {
  assert(frames_ != 0);
  assert(delay_ != 0);
}

inline void Sprite::DelayAnim::incr() {
  assert(frame_ < frames_ * delay_);
  ++frame_;
}

inline void Sprite::DelayAnim::incrRepeat() {
  assert(frame_ < frames_ * delay_);
  frame_ = (frame_ + 1) % (frames_ * delay_);
}

inline bool Sprite::DelayAnim::incrStop() {
  assert(frame_ < frames_ * delay_);
  ++frame_;
  const bool end = (frame_ == (frames_ * delay_));
  frame_ -= end;
  return end;
}

inline void Sprite::DelayAnim::delay(const ID delay) {
  assert(delay != 0);
  if (delay_ != delay) {
    frame_ /= delay_;
    frame_ *= delay;
    delay_ = delay;
  }
}

inline void Sprite::DelayAnim::noDelay() {
  delay(1);
}

inline void Sprite::DelayAnim::maxDelay() {
  delay(~ID{} / frames_);
}

inline void Sprite::DelayAnim::speed(const double speed) {
  assert(speed > 0.0);
  delay(1.0 / speed + 0.5);
}

inline Sprite::ID Sprite::DelayAnim::firstSprite() const {
  return sprite_;
}

inline Sprite::ID Sprite::DelayAnim::sprite() const {
  assert(frame_ < frames_ * delay_);
  assert(sprite_ != NULL_SPRITE);
  return sprite_ + frame_ / delay_;
}

inline Sprite::ID Sprite::DelayAnim::sprite(const ID group) const {
  return sprite() + frames_ * group;
}

template <typename T>
Sprite::ID Sprite::DelayAnim::sprite(const T group) const {
  return sprite(static_cast<ID>(group));
}
