//
//  linear zoom.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_linear_zoom_hpp
#define engine_camera_2d_linear_zoom_hpp

#include "zoom.hpp"

namespace Cam2D {
  class LinearZoom final : public Zoom {
  public:
    explicit LinearZoom(float);
    
    float calcPPM(Props, float, float) override;
  
  private:
    float speed = 1.0f;
  }
}

#endif
