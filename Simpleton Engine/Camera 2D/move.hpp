//
//  move.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_move_hpp
#define engine_camera_2d_move_hpp

#include <glm/vec2.hpp>

namespace Cam2D {
  struct Props;
  
  class Move {
  public:
    Move() = default;
    virtual ~Move() = default;
    
    virtual glm::vec2 calcCenter(Props, glm::vec2 target, float delta) = 0;
  };
}

#endif
