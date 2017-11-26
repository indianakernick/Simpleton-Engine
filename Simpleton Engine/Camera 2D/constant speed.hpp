//
//  constant speed.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 26/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_constant_speed_hpp
#define engine_camera_2d_constant_speed_hpp

#include "animate.hpp"
#include <glm/gtx/norm.hpp>

namespace Cam2D {
  template <PropID PROP>
  class ConstantSpeed final : public Animate<PROP> {
  public:
    using Type = PropType<PROP>;
    
    ConstantSpeed()
      : speed(1.0f) {}
    explicit ConstantSpeed(const float speed)
      : speed(speed) {}
    
    Type calculate(const Props props, const Type target, const float delta) override {
      const Type toTarget = target - getProp<PROP>(props);
      const float targetDist = glm::length(toTarget);
      const float deltaDist = speed * delta;
      if (targetDist <= deltaDist) {
        return target;
      } else {
        return getProp<PROP>(props) + deltaDist * toTarget / targetDist;
      }
    }
  
  private:
    float speed;
  };
  
  using PosConstantSpeed = ConstantSpeed<PropID::POS>;
  using ZoomConstantSpeed = ConstantSpeed<PropID::ZOOM>;
}

#endif
