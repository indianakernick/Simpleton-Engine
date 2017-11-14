//
//  props.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_props_hpp
#define engine_camera_2d_props_hpp

#include <glm/vec2.hpp>

namespace Cam2D {
  struct Props {
    glm::vec2 center = {0.0f, 0.0f};
    glm::ivec2 windowSize;
    ///Pixels per meter
    float ppm = 128.0f;
  };
}

#endif
