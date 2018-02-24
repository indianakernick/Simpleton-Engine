//
//  blit images.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 23/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "blit images.hpp"

#include <cassert>
#include <iostream>
#include <Simpleton/Utils/profiler.hpp>
#include <Simpleton/Graphics 2D/blit surface.hpp>

G2D::Surface blitImages(
  const std::vector<G2D::Surface> &images,
  const std::vector<stbrp_rect> &rects,
  const stbrp_coord length
) {
  assert(images.size() == rects.size());
  PROFILE(blitImages);
  
  std::cout << "Copying smaller images onto larger image\n";
  
  G2D::Surface image(length, length, 4);
  
  for (size_t i = 0; i != images.size(); ++i) {
    blit(image, images[i], rects[i].x, rects[i].y);
  }
  
  return image;
}
