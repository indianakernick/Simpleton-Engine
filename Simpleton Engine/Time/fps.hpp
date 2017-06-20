//
//  fps.hpp
//  Game Engine
//
//  Created by Indi Kernick on 16/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_time_fps_hpp
#define engine_time_fps_hpp

#include "get.hpp"
#include <queue>
#include <cassert>

namespace Time {
  class FPS {
  public:
    explicit FPS(uint8_t avgSeconds = 1);
    
    ///Call this when you want to start recording FPS
    void init();
    ///Call this every frame
    void frame();
    ///Get the average number of frames
    double get();
  private:
    //time init was called in nanoseconds
    uint64_t startTime = 0;
    uint16_t totalFrames;
    std::queue<uint8_t> frames;
  };
};

#endif
