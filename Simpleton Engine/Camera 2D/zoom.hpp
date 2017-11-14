//
//  zoom.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_zoom_hpp
#define engine_camera_2d_zoom_hpp

namespace Cam2D {
  struct Props;
  
  class Zoom {
  public:
    Zoom() = default;
    virtual ~Zoom() = default;
    
    virtual float calcPPM(Props, float target, float delta) = 0;
  };
}

#endif
