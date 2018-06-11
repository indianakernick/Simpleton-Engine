//
//  anim.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 10/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_sprite_anim_hpp
#define engine_sprite_anim_hpp

#include "id.hpp"
#include <cassert>

namespace Sprite::detail {
  class BaseAnim {
  protected:
    BaseAnim()
      : sprite_{NULL_SPRITE}, frames_{0}, frame_{0} {}
    BaseAnim(const ID sprite, const ID frames)
      : sprite_{sprite}, frames_{frames}, frame_{0} {
      assert(sprite_ != NULL_SPRITE);
      assert(frames_ != 0);
    }
  
    ID sprite_;
    ID frames_;
    ID frame_;
    
    ID endFrame() const {
      return frames_;
    }
    ID frame() const {
      return frame_;
    }
  };
  
  template <typename Base = BaseAnim>
  class DelayAnim : public Base {
    using BaseAnim::frame_;
    using BaseAnim::frames_;
  
  public:
    DelayAnim()
      : Base{}, delay_{0} {}
    DelayAnim(const ID sprite, const ID frames, const ID delay = 1)
      : Base{sprite, frames}, delay_{delay} {
      assert(delay_ != 0);
    }
    
    void delay(const ID delay) {
      assert(delay != 0);
      if (delay_ != delay) {
        frame_ /= delay_;
        frame_ *= delay;
        delay_ = delay;
      }
    }
    void noDelay() {
      delay(1);
    }
    void maxDelay() {
      delay(~ID{} / frames_);
    }
    void speed(const float speed) {
      assert(speed > 0.0f);
      delay(1.0f / speed + 0.5f);
    }
    
  protected:
    ID endFrame() const {
      return Base::endFrame() * delay_;
    }
    ID frame() const {
      return Base::frame() / delay_;
    }
  
  private:
    ID delay_;
  };

  template <typename Base = BaseAnim>
  class Anim : public Base {
    using BaseAnim::frame_;
    using BaseAnim::frames_;
    using BaseAnim::sprite_;
    using Base::endFrame;
    using Base::frame;
  
  public:
    Anim() = default;
    Anim(const ID sprite, const ID frames)
      : Base{sprite, frames} {}
    
    void incr() {
      assert(frame_ < endFrame());
      ++frame_;
    }
    void incrRepeat() {
      assert(frame_ < endFrame());
      frame_ = (frame_ + 1) % endFrame();
    }
    bool incrStop() {
      assert(frame_ < endFrame());
      ++frame_;
      const bool end = frame_ == endFrame();
      frame_ -= end;
      return end;
    }
    
    ID firstSprite() const {
      return sprite_;
    }
    ID sprite() const {
      assert(frame_ < endFrame());
      assert(sprite_ != NULL_SPRITE);
      return sprite_ + frame();
    }
    ID sprite(const ID group) const {
      return sprite() + endFrame() * group;
    }
    template <typename T>
    ID sprite(const T group) const {
      return sprite(static_cast<ID>(group));
    }
    
    float progress() const {
      assert(frames_ != 0);
      if (frames_ == 1) {
        return 0.0f;
      } else {
        return static_cast<float>(frame_) / (frames_ - 1);
      }
    }
    
    bool firstFrame() const {
      return frame_ == 0;
    }
    bool lastFrame() const {
      return frame_ == frames_ - 1;
    }
  };
}

namespace Sprite {
  using Anim = detail::Anim<>;
  using DelayAnim = detail::Anim<detail::DelayAnim<>>;
}

#include "anim.inl"

#endif
