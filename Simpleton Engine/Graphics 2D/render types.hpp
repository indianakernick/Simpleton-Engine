//
//  render types.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 18/3/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_render_types_hpp
#define engine_graphics_2d_render_types_hpp

#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>

namespace G2D {
  using TextureID = size_t;
  
  struct QuadRange {
    size_t begin;
    size_t end;
    
    size_t size() const {
      return end - begin;
    }
  };
  
  struct RenderParams {
    glm::mat3 viewProj {
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f
    };
    TextureID tex {0};
    glm::vec4 color {1.0f};
  };
}

#endif
