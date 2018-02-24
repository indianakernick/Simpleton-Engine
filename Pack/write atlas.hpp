//
//  write atlas.hpp
//  Pack
//
//  Created by Indi Kernick on 24/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef write_atlas_hpp
#define write_atlas_hpp

#include <vector>
#include <string_view>
#include "stb_rect_pack.h"

void writeAtlas(
  std::string_view,
  const std::vector<std::string> &,
  const std::vector<stbrp_rect> &,
  stbrp_coord,
  stbrp_coord
);

#endif
