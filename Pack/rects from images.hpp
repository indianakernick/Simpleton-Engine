//
//  rects from images.hpp
//  Pack
//
//  Created by Indi Kernick on 24/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef rects_from_images_hpp
#define rects_from_images_hpp

#include <vector>
#include "stb_rect_pack.h"
#include <Simpleton/Graphics 2D/surface.hpp>

std::vector<stbrp_rect> rectsFromImages(
  const std::vector<G2D::Surface> &,
  stbrp_coord
);

#endif
