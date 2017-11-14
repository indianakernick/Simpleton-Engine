//
//  target pos.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_target_pos_hpp
#define engine_camera_2d_target_pos_hpp

#include <glm/vec2.hpp>

namespace Cam2D {
  struct Props;
  
  class TargetPos {
  public:
    TargetPos() = default;
    virtual ~TargetPos() = default;
    
    virtual glm::vec2 calcTarget(Props) = 0;
  };
}

#endif
