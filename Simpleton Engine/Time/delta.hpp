//
//  delta.hpp
//  Game Engine
//
//  Created by Indi Kernick on 22/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_time_delta_hpp
#define engine_time_delta_hpp

#include "get.hpp"

namespace Time {
  template <typename DURATION_TYPE>
  class Delta {
  public:
    Delta()
      : lastTime(getPoint<DURATION_TYPE>()) {}
    
    ///Get the duration that has passed since the last call of this function
    uint64_t get() {
      Point<DURATION_TYPE> now = getPoint<DURATION_TYPE>();
      DURATION_TYPE delta = now - lastTime;
      lastTime = now;
      return delta.count();
    }
  private:
    Point<DURATION_TYPE> lastTime;
  };
}

#endif
