//
//  zsort.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 21/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_zsort_hpp
#define engine_graphics_2d_zsort_hpp

#include "types.hpp"

namespace G2D {
  /// Sort by the depth of the first vertex
  void sort(Quad *, Quad *);
  /// Sort by the average depth of the verticies
  void sortCenter(Quad *, Quad *);
}

#include "zsort.inl"

#endif
