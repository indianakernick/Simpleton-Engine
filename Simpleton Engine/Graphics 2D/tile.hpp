//
//  tile.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 21/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_tile_hpp
#define engine_graphics_2d_tile_hpp

#include "types.hpp"

namespace G2D {
  void tilePos(Quad &, float depth, glm::vec2 pos, glm::vec2 size = {1.0f, 1.0f});
  void tileTex(Quad &, TexCoordType pos, TexCoordType size = {1.0f, 1.0f});
}

#include "tile.inl"

#endif
