//
//  zoom to fit.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_zoom_to_fit_hpp
#define engine_camera_2d_zoom_to_fit_hpp

#include "target.hpp"
#include <glm/vec2.hpp>

namespace Cam2D {
  class ZoomToFit final : public Target<PropID::ZOOM> {
  public:
    explicit ZoomToFit(glm::vec2);
    
    void setSize(glm::vec2);
    
    float calcTarget(Props) override;
    
  private:
    glm::vec2 box;
  };
}

#include "zoom to fit.inl"

#endif
