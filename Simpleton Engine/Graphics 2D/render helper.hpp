//
//  render helper.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/4/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_render_helper_hpp
#define engine_graphics_2d_render_helper_hpp

#include "renderer.hpp"
#include "job manager.hpp"

namespace G2D {
  void render(Renderer &, JobManager &);
}

#include "render helper.inl"

#endif
