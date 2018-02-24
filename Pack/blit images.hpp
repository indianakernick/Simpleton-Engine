//
//  blit images.hpp
//  Pack
//
//  Created by Indi Kernick on 23/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef blit_images_hpp
#define blit_images_hpp

#include <vector>
#include "stb_rect_pack.h"
#include <Simpleton/Graphics 2D/surface.hpp>

G2D::Surface blitImages(
  const std::vector<G2D::Surface> &,
  const std::vector<stbrp_rect> &,
  stbrp_coord
);

#endif
