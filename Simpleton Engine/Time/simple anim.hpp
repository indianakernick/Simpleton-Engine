//
//  simple anim.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 6/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_time_simple_anim_hpp
#define engine_time_simple_anim_hpp

#include "../Math/int float.hpp"

namespace Time {
  template <typename Duration>
  class SimpleAnim {
  public:
    static_assert(std::is_arithmetic<Duration>::value);
  
    SimpleAnim() = default;
    explicit SimpleAnim(const Duration duration)
      : duration(duration) {}
  
    void advance(const Duration delta) {
      progress += delta;
    }
    
    bool overflow() const {
      return progress > duration;
    }
    void stopOnEdge() {
      if (progress > duration) {
        progress = duration;
      }
    }
    void repeat() {
      if (duration == Duration(0)) {
        progress = Duration(0);
      } else {
        progress = Math::mod(progress, duration);
      }
    }
    
    void reset() {
      progress = Duration(0);
    }
    void reset(const Duration newDuration) {
      progress = Duration(0);
      duration = newDuration;
    }
    
    template <typename Float>
    std::enable_if_t<
      std::is_floating_point<Float>::value,
      Float
    >
    getProgress() const {
      if (duration == Duration(0)) {
        if (progress == Duration(0)) {
          return Float(0);
        } else {
          return Math::infinity<Float>();
        }
      } else {
        return static_cast<Float>(progress) / static_cast<Float>(duration);
      }
    }
  
  private:
    Duration duration = Duration(0);
    Duration progress = Duration(0);
  };
}

#endif
