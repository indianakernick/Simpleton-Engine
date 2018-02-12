//
//  camera.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_camera_hpp
#define engine_camera_2d_camera_hpp

#include <memory>
#include "target.hpp"
#include "animate.hpp"
#include "transform.hpp"

namespace Cam2D {
  class Camera {
  public:
    explicit Camera(float = 1.0f);
    Camera(glm::vec2, float);
    
    void setPos(glm::vec2);
    void setZoom(float);
    void setAngle(float);
    
    template <PropID ...TARGET_PROPS, PropID ...ANIM_PROPS>
    void update(Params, Target<TARGET_PROPS> &..., Animate<ANIM_PROPS> &...);
    template <PropID ...TARGET_PROPS>
    void update(Params, Target<TARGET_PROPS> &...);

    Transform transform;
    
  private:
    Props props;
  };
}

#include "camera.inl"

#endif
