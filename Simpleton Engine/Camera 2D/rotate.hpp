//
//  rotate to.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 11/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_rotate_hpp
#define engine_camera_2d_rotate_hpp

#include "target.hpp"
#include <glm/vec2.hpp>

namespace Cam2D {
  class Rotate final : public Target<PropID::ANGLE> {
  public:
    enum class Dir {
      /// Counter-clockwise (anti-clockwise)
      CCW,
      /// Clockwise
      CW
    };
  
    Rotate() = default;
    Rotate(Dir, bool);
    
    void setDir(Dir);
    void setEnabled(bool);
    
    float calcTarget(Props) override;
  
  private:
    float lastAngle = 0.0f;
    Dir dir = Dir::CCW;
    bool enabled = true;
  };
}

#include "rotate.inl"

#endif
