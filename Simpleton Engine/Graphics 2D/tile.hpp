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
  /// Write positions of verticies on a quad assuming that the quad is an
  /// axis-aligned rectangle
  void tilePos(Quad &, float depth, glm::vec2 pos, glm::vec2 size = {1.0f, 1.0f});
  /// Write texture coordinates of vertices on a quad assuming that the texture
  /// is sampled as an axis-aligned rectangle
  void tileTex(Quad &, TexCoordType min, TexCoordType max);
}

#include "tile.inl"

#endif
