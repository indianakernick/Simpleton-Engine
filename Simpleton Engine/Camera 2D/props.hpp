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
    ///Size of the window
    glm::ivec2 windowSize;
    ///Position in meters of the center of the camera
    glm::vec2 center;
    ///Pixels per meter
    float ppm;
  };
}

#endif
