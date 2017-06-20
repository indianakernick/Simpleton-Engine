//
//  frame anim.hpp
//  Game Engine
//
//  Created by Indi Kernick on 21/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_time_frame_anim_hpp
#define engine_time_frame_anim_hpp

#include <cstdint>
#include <cmath>

namespace Time {
  class FrameAnim {
  public:
    FrameAnim(uint8_t animFPS, uint16_t animLength, uint8_t displayFPS = 60);
    
    ///Call this every frame to increment the frame counter
    void update();
    ///Returns the current frame of the animation
    uint16_t getFrame();
  private:
    uint8_t animFPS;
    uint16_t animLength;
    uint8_t displayFPS;
    uint32_t frame = 0;
  };
}

#endif
