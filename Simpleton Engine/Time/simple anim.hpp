//
//  simple anim.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 6/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_time_simple_anim_hpp
#define engine_time_simple_anim_hpp

#include <limits>
#include <type_traits>

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
      assert(duration != Duration(0));
      
      if constexpr (std::is_floating_point<Duration>::value) {
        Duration quot = progress / duration;
        quot -= static_cast<int>(quot);
        return quot * duration;
      } else {
        progress %= duration;
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
        using Limits = std::numeric_limits<Float>;
        if constexpr (Limits::has_infinity()) {
          return Limits::infinity();
        } else {
          return Limits::max();
        }
      }
      return static_cast<Float>(progress) / static_cast<Float>(duration);
    }
  
  private:
    Duration duration = Duration(0);
    Duration progress = Duration(0);
  };
}

#endif
