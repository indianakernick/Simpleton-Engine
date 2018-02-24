//
//  rects from images.cpp
//  Pack
//
//  Created by Indi Kernick on 24/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include "rects from images.hpp"

std::vector<stbrp_rect> rectsFromImages(
  const std::vector<G2D::Surface> &images,
  const stbrp_coord sep
) {
  std::vector<stbrp_rect> rects;
  rects.reserve(images.size());
  for (auto i = images.cbegin(); i != images.cend(); ++i) {
    stbrp_rect rect;
    rect.w = i->width() + sep;
    rect.h = i->height() + sep;
    rects.push_back(rect);
  }
  return rects;
}
