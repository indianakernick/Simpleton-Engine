//
//  pack rects.hpp
//  Pack
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef pack_rects_hpp
#define pack_rects_hpp

#include <vector>
#include "stb_rect_pack.h"

class RectPackError final : public std::runtime_error {
public:
  RectPackError();
};

stbrp_coord packRects(std::vector<stbrp_rect> &);

#endif
