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
    ~RealFreqLimiter() = default;
    
    ///Change the duration
    void setDuration(const Duration newDuration) {
      duration = newDuration;
    }
    
    ///Get the duration
    Duration getDuration() const {
      return duration;
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
    
    ///Returns the number of times the operation can be performed
    uint64_t canDoMultiple() {
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
        count++;
      }
      return count;
    }
    
  private:
    //minumum duration between operations
    Duration duration;
    //time of last operation
    Point<Duration> lastDo;
  };
  
  ///Limits the frequency of some operation in delta-time
  template <typename Number>
  class DeltaFreqLimiter {
    
    static_assert(std::is_arithmetic<Number>::value);
  
  public:
    DeltaFreqLimiter()
      : duration(0) {}
    explicit DeltaFreqLimiter(const Number duration)
      : duration(duration) {}
    ~DeltaFreqLimiter() = default;
    
    ///Change the duration
    void setDuration(const Number newDuration) {
      duration = newDuration;
    }
    
    ///Get the duration
    Number getDuration() const {
      return duration;
    }
    
    ///Advance time forward
    void advance(const Number delta) {
      timeSinceLast += delta;
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
    
    ///Returns the number of times the operation can be performed
    Number canDoMultiple() {
      if (duration == Number(0)) {
        if (timeSinceLast == Number(0)) {
          return Number(0);
        } else {
          return std::numeric_limits<Number>::max();
        }
      }
      Number count = 0;
      while (timeSinceLast >= duration) {
        count++;
        timeSinceLast -= duration;
      }
      return count;
    }
    
  private:
    //minumum duration between operations
    Number duration;
    //time since last operation
    Number timeSinceLast = 0;
  };
}

#endif
