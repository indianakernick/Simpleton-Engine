//
//  stopwatch.hpp
//  Game Engine
//
//  Created by Indi Kernick on 28/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_time_stopwatch_hpp
#define engine_time_stopwatch_hpp

#include "get.hpp"
#include <cassert>

namespace Time {
  ///A stopwatch to record durations
  template <typename DURATION_TYPE>
  class StopWatch {
  private:
    enum State : char {
      INITIAL,
      RUNNING,
      STOPPED
    };
  public:
    StopWatch() = default;
    explicit StopWatch(bool)
      : startPoint(getI<DURATION_TYPE>()), endPoint(0), state(RUNNING) {}
    
    ///Starts the stopwatch
    void start() {
      assert(state != RUNNING);
      state = RUNNING;
      startPoint = getI<DURATION_TYPE>();
    }
    ///Stops the stopwatch and returns the duration
    uint64_t stop() {
      assert(state == RUNNING);
      state = STOPPED;
      endPoint = getI<DURATION_TYPE>();
      return endPoint - startPoint;
    }
    ///Resume the stopwatch after it has been stopped, returning the duration
    uint64_t resume() {
      assert(state == STOPPED);
      state = RUNNING;
      startPoint += getI<DURATION_TYPE>() - endPoint;
      return endPoint - startPoint;
    }
    ///Gets the duration. Can be called while the stopwatch is running
    uint64_t get() {
      switch (state) {
        case INITIAL:
          return 0;
        case RUNNING:
          return getI<DURATION_TYPE>() - startPoint;
        case STOPPED:
          return endPoint - startPoint;
      }
    }
    ///Returns the duration and resets the stopwatch
    uint64_t lap() {
      assert(state == RUNNING);
      uint64_t now = getI<DURATION_TYPE>();
      uint64_t duration = now - startPoint;
      startPoint = now;
      return duration;
    }
    ///Returns whether the stopwatch is running (start has just been called).
    bool running() {
      return state == RUNNING;
    }
  private:
    uint64_t startPoint = 0;
    uint64_t endPoint = 0;
    State state = INITIAL;
  };
}

#endif
