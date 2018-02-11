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
    
    void update(float windowAspectRatio, float deltaTime);

    Transform transform;
    
    std::unique_ptr<Animate<PropID::POS>> animatePos;
    std::unique_ptr<Animate<PropID::ZOOM>> animateZoom;
    std::unique_ptr<Animate<PropID::ANGLE>> animateAngle;
    
    std::unique_ptr<Target<PropID::POS>> targetPos;
    std::unique_ptr<Target<PropID::ZOOM>> targetZoom;
    std::unique_ptr<Target<PropID::ANGLE>> targetAngle;

  private:
    Props props;
  };
}

#include "camera.inl"

#endif
