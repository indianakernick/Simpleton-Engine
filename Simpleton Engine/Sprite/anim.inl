//
//  anim.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 10/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include <cassert>

/*Sprite::Anim::Anim() {}

Sprite::ID Sprite::Anim::sprite() const {
  assert(frame_ < Base::endFrame());
  assert(sprite_ != NULL_SPRITE);
  return sprite_ + Base::frame();
}

inline void f() {
  Sprite::Anim<0> anim;
  anim.sprite();
}

template <Sprite::ID Delay>
inline Sprite::Anim::Anim()
  : sprite_{NULL_SPRITE}, frames_{0}, frame_{0} {}

template <Sprite::ID Delay>
inline Sprite::Anim::Anim(const ID sprite, const ID frames)
  : sprite_{sprite}, frames_{frames}, frame_{0} {
  assert(frames_ != 0);
}

template <Sprite::ID Delay>
inline void Sprite::Anim::incr() {
  assert(frame_ < frames_);
  ++frame_;
}

template <Sprite::ID Delay>
inline void Sprite::Anim::incrRepeat() {
  assert(frame_ < frames_);
  frame_ = (frame_ + 1) % frames_;
}

template <Sprite::ID Delay>
inline bool Sprite::Anim::incrStop() {
  assert(frame_ < frames_);
  ++frame_;
  const bool end = (frame_ == frames_);
  frame_ -= end;
  return end;
}

template <Sprite::ID Delay>
inline Sprite::ID Sprite::Anim::firstSprite() const {
  return sprite_;
}

template <Sprite::ID Delay>
inline Sprite::ID Sprite::Anim::sprite() const {
  assert(frame_ < frames_);
  assert(sprite_ != NULL_SPRITE);
  return sprite_ + frame_;
}

template <Sprite::ID Delay>
inline Sprite::ID Sprite::Anim::sprite(const ID group) const {
  return sprite() + frames_ * group;
}

template <Sprite::ID Delay>
template <typename T>
Sprite::ID Sprite::Anim::sprite(const T group) const {
  return sprite(static_cast<ID>(group));
}

template <Sprite::ID Delay>
inline Sprite::ID Sprite::Anim::frame() const {
  return frame_;
}

template <Sprite::ID Delay>
inline float Sprite::Anim::progress() const {
  assert(frames_ != 0);
  if (frames_ == 1) {
    return 0.0f;
  } else {
    return static_cast<float>(frame_) / (frames_ - 1);
  }
}

template <Sprite::ID Delay>
inline bool Sprite::Anim::firstFrame() const {
  return frame_ == 0;
}

inline bool Sprite::Anim::lastFrame() const {
  return frame_ == frames_ - 1;
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

inline bool Sprite::DelayAnim::firstFrame() const {
  return frame_ == 0;
}

inline bool Sprite::DelayAnim::lastFrame() const {
  return frame_ == frames_ * delay_;
}
*/
