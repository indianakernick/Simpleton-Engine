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
    
    void update(glm::ivec2 windowSize, float delta);

    Transform transform;
    std::unique_ptr<Animate<PropID::POS>> animatePos;
    std::unique_ptr<Animate<PropID::ZOOM>> animateZoom;
    std::unique_ptr<Target<PropID::POS>> targetPos;
    std::unique_ptr<Target<PropID::ZOOM>> targetZoom;

  private:
    Props props;
  };
}

#endif
