//
//  timer.hpp
//  Game Engine
//
//  Created by Indi Kernick on 29/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_time_timer_hpp
#define engine_time_timer_hpp

#include "get.hpp"
#include <cassert>

namespace Time {
  ///A timer to count down for duration
  template <typename DURATION_TYPE>
  class Timer {
  private:
    enum State : char {
      INITIAL,
      RUNNING,
      PAUSED,
      DONE
    };
  public:
    Timer() = default;
    
    ///Sets the duration to be counted down from when the timer starts
    void setDuration(uint64_t newDuration) {
      assert(state == INITIAL || state == DONE);
      duration = newDuration;
    }
    ///Starts the timer with the set duration
    void start() {
      assert(state == INITIAL || state == DONE);
      state = RUNNING;
      startPoint = getI<DURATION_TYPE>();
    }
    ///Starts the timer with the duration given
    void start(uint64_t newDuration) {
      assert(state == INITIAL || state == DONE);
      state = RUNNING;
      duration = newDuration;
      startPoint = getI<DURATION_TYPE>();
    }
    ///Pause the timer and returns the time left
    uint64_t pause() {
      assert(state == RUNNING);
      state = PAUSED;
      uint64_t now = getI<DURATION_TYPE>();
      pausePoint = now;
      return duration - (now - startPoint);
    }
    ///Resume the timer and returns the time left
    uint64_t resume() {
      assert(state == PAUSED);
      state = RUNNING;
      uint64_t now = getI<DURATION_TYPE>();
      //move the startPoint forward so that timeLeft is the same as it was
      //when it was paused
      startPoint += now - pausePoint;
      return duration - (now - startPoint);
    }
    ///Get the amount of time left before the timer is done
    uint64_t get() {
      assert(state != INITIAL);
      
      if (state == INITIAL || state == DONE) {
        return 0;
      }
      
      uint64_t now = getI<DURATION_TYPE>();
      
      if (state == PAUSED) {
        startPoint += now - pausePoint;
        pausePoint = now;
      }
      
      uint64_t timeLeft = duration - (now - startPoint);
      //if timeLeft overflows then we've passed the duration
      if (timeLeft > duration) {
        state = DONE;
        return 0;
      } else {
        return timeLeft;
      }
    }
    ///Checks whether the timer is done
    bool done() {
      get();
      return state == DONE;
    }
  private:
    uint64_t duration = 0;
    uint64_t startPoint = 0;
    uint64_t pausePoint = 0;
    State state = INITIAL;
  };
}

#endif
