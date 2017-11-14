//
//  transform.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_transform_hpp
#define engine_camera_2d_transform_hpp

#include "aabb.hpp"
#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>

namespace Cam2D {
  struct Props;
  
  class Transform {
  public:
    Transform() = default;
    
    glm::mat3 toPixels() const;
    glm::mat3 toMeters() const;
    bool visibleMeters(AABB) const;
    
    void calculate(Props);
    
  private:
    glm::mat3 toPixelsMat;
    glm::mat3 toMetersMat;
    AABB windowBounds;
  };
}

#endif
