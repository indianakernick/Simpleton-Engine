//
//  target scale.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_target_scale_hpp
#define engine_camera_2d_target_scale_hpp

namespace Cam2D {
  struct Props;
  
  class TargetScale {
  public:
    TargetScale() = default;
    virtual ~TargetScale() = default;
    
    virtual float calcTarget(Props) = 0;
  };
}

#endif
