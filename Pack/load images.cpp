//
//  load images.cpp
//  Pack
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "load images.hpp"

#include <iostream>
#include <Simpleton/Graphics 2D/load surface.hpp>

NoLoadedImagesError::NoLoadedImagesError()
  : std::runtime_error("No images successfully loaded") {}

std::vector<G2D::Surface> loadImages(const std::vector<std::string> &files) {
  std::vector<G2D::Surface> images;
  images.reserve(files.size());
  
  for (auto f = files.cbegin(); f != files.cend(); ++f) {
    try {
      std::cout << "Loading image \"" << *f << "\"\n";
      images.emplace_back(G2D::loadSurfaceRGBA(*f));
    } catch (G2D::SurfaceLoadError &e) {
      std::cerr << e.what() << '\n';
    }
  }
  
  if (images.size() == 0) {
    throw NoLoadedImagesError();
  }
  
  return images;
}
