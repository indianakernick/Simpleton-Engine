//
//  load images.cpp
//  Pack
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "load images.hpp"

#include <iostream>
#include <Simpleton/Utils/profiler.hpp>
#include <Simpleton/Graphics 2D/load surface.hpp>

std::vector<G2D::Surface> loadImages(
  const std::vector<std::string> &files,
  const int bpp
) {
  PROFILE(loadImages);
  
  std::vector<G2D::Surface> images;
  images.reserve(files.size());
  
  for (auto f = files.cbegin(); f != files.cend(); ++f) {
    images.emplace_back(G2D::loadSurface(*f, bpp));
  }
  
  return images;
}
