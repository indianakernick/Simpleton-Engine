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
#include "move.hpp"
#include "zoom.hpp"
#include "props.hpp"
#include "transform.hpp"
#include "target pos.hpp"
#include "target scale.hpp"

namespace Cam2D {
  class Camera {
  public:
    Camera() = default;
    
    Transform transform;
    std::unique_ptr<Move> move;
    std::unique_ptr<Zoom> zoom;
    std::unique_ptr<TargetPos> targetPos;
    std::unique_ptr<TargetScale> targetScale;
    
    void update(glm::ivec2 windowSize, float delta);
    
  private:
    Props props;
  };
}

#endif
