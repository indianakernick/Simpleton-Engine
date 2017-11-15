//
//  linear move.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_linear_move_hpp
#define engine_camera_2d_linear_move_hpp

#include "move.hpp"

namespace Cam2D {
  class LinearMove final : public Move {
  public:
    explicit LinearMove(float);
    
    glm::vec2 calcCenter(Props, glm::vec2, float) override;
    
  private:
    float speed = 1.0f;
  };
}

#endif
