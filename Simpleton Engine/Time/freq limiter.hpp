//
//  freq limiter.hpp
//  Game Engine
//
//  Created by Indi Kernick on 21/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_time_freq_limiter_hpp
#define engine_time_freq_limiter_hpp

#include "get.hpp"

namespace Time {
  class OpPerSec {};
  
  constexpr OpPerSec OP_PER_SEC {};

  ///Limits the frequency of some operation in real-time
  template <typename Duration>
  class RealFreqLimiter {
  public:
    RealFreqLimiter()
      : duration(0),
        lastDo(getPoint<Duration>()) {}
    template <typename Int>
    explicit RealFreqLimiter(const Int count)
      : duration(count),
        lastDo(getPoint<Duration>() - duration) {}
    template <typename Int>
    RealFreqLimiter(OpPerSec, const Int count)
      : duration((static_cast<float>(Duration::period::num) / Duration::period::den) * count),
        lastDo(getPoint<Duration>() - duration) {}
    ~RealFreqLimiter() = default;
    
    ///Change the duration
    void setDuration(const Duration newDuration) {
      duration = newDuration;
    }
    
    ///Get the duration
    Duration getDuration() const {
      return duration;
    }
    
    ///Get the amount of time that has passes since the last operation
    Duration getTimeSinceLast() const {
      return getPoint<Duration>() - lastDo;
    }
    
    ///Get the progress of the cycle
    template <typename Float>
    std::enable_if_t<
      std::is_floating_point<Float>::value,
      Float
    >
    getProgress() const {
      return static_cast<Float>((getPoint<Duration>() - lastDo).count()) / duration.count();
    }
    
    ///Simulate a fake operation
    void reset() {
      lastDo = getPoint<Duration>();
    }
    
    ///Ensure that canDo will return true
    void willDo() {
      lastDo = getPoint<Duration>() - duration;
    }
    
    ///If this function returns true, it will not return true again until the
    ///duration has passed
    bool canDo() {
      const Point<Duration> now = getPoint<Duration>();
      if (now - lastDo >= duration) {
        lastDo = now;
        return true;
      } else {
        return false;
      }
    }
    
    ///Similar to canDo but...
    bool canDoOverlap() {
      const Point<Duration> now = getPoint<Duration>();
      if (now - lastDo >= duration) {
        lastDo += duration;
        return true;
      } else {
        return false;
      }
    }
    
    ///Returns the number of times the operation can be performed
    uint64_t canDoMultiple() {
      return canDoMultipleImpl<false>();
    }
    
    ///Similar to canDoMultiple but...
    uint64_t canDoMultipleOverlap() {
      return canDoMultipleImpl<true>();
    }
    
  private:
    //minumum duration between operations
    Duration duration;
    //time of last operation
    Point<Duration> lastDo;
    
    template <bool OVERLAP>
    uint64_t canDoMultipleImpl() {
      const Point<Duration> now = getPoint<Duration>();
      
      if (duration == 0) {
        if (now - lastDo == Duration(0)) {
          return 0;
        } else {
          return std::numeric_limits<uint64_t>::max();
        }
      }
    
      uint64_t count = 0;
      while (now - lastDo >= duration) {
        lastDo += duration;
        ++count;
      }
      if constexpr (!OVERLAP) {
        lastDo = now;
      }
      return count;
    }
  };
  
  ///Limits the frequency of some operation in delta-time
  template <typename Number>
  class DeltaFreqLimiter {
    
    static_assert(std::is_arithmetic<Number>::value);
  
  public:
    DeltaFreqLimiter()
      : duration(0) {}
    explicit DeltaFreqLimiter(const Number duration)
      : duration(duration) {}                                  //assumes milliseconds
    DeltaFreqLimiter(OpPerSec, const Number count, const Number unitsPerSec = Number(1000))
      : duration(unitsPerSec / count) {}
    ~DeltaFreqLimiter() = default;
    
    ///Change the duration
    void setDuration(const Number newDuration) {
      duration = newDuration;
    }
    
    ///Get the duration
    Number getDuration() const {
      return duration;
    }
    
    ///Get the amount of time that has passes since the last operation
    Number getTimeSinceLast() const {
      return timeSinceLast;
    }
    
    ///Get the progress of the cycle
    template <typename Float>
    std::enable_if_t<
      std::is_floating_point<Float>::value,
      Float
    >
    getProgress() const {
      return static_cast<Float>(timeSinceLast) / duration;
    }
    
    ///Advance time forward
    void advance(const Number delta) {
      timeSinceLast += delta;
    }
    
    ///Simulate a fake operation
    void reset() {
      timeSinceLast = 0;
    }
    
    ///Ensure that canDo will return true
    void willDo() {
      timeSinceLast = duration;
    }
    
    ///If this function returns true, it will not return true again until the
    ///duration has passed
    bool canDo() {
      if (timeSinceLast >= duration) {
        timeSinceLast = Number(0);
        return true;
      } else {
        return false;
      }
    }
    
    ///Similar to canDo but...
    bool canDoOverlap() {
      if (timeSinceLast >= duration) {
        timeSinceLast -= duration;
        return true;
      } else {
        return false;
      }
    }
    
    using CountType = std::conditional_t<std::is_integral<Number>::value, Number, uint32_t>;
    
    ///Returns the number of times the operation can be performed
    CountType canDoMultiple() {
      return canDoMultipleImpl<false>();
    }
    
    ///Similar to canDoMultiple but...
    CountType canDoMultipleOverlap() {
      return canDoMultipleImpl<true>();
    }
    
  private:
    //minumum duration between operations
    Number duration;
    //time since last operation
    Number timeSinceLast = Number(0);
    
    template <bool OVERLAP>
    CountType canDoMultipleImpl() {
      if (duration == Number(0)) {
        if (timeSinceLast == Number(0)) {
          return Number(0);
        } else {
          using Limits = std::numeric_limits<Number>;
          return Limits::has_infinity ? Limits::infinity() : Limits::max();
        }
      }
      CountType count = 0;
      while (timeSinceLast >= duration) {
        timeSinceLast -= duration;
        ++count;
      }
      if constexpr (!OVERLAP) {
        timeSinceLast = Number(0);
      }
      return count;
    }
  };
}

#endif
