//
//  pack rects.cpp
//  Pack
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "pack rects.hpp"

#include <cmath>
#include <iostream>

RectPackError::RectPackError()
  : std::runtime_error("Failed to pack rectangles") {}

uint32_t calcArea(const std::vector<stbrp_rect> &rects) {
  uint32_t area = 0;
  for (auto i = rects.cbegin(); i != rects.cend(); ++i) {
    area += i->w * i->h;
  }
  return area;
}

stbrp_coord ceilToPowerOf2(const stbrp_coord num) {
  const unsigned long long numl = num;
  return (1 << (sizeof(long long) * CHAR_BIT - __builtin_clzll(numl - 1))) - (numl == 1);
}

stbrp_coord calcLength(const uint32_t area) {
  const stbrp_coord length = static_cast<stbrp_coord>(std::ceil(std::sqrt(area)));
  const stbrp_coord ceiledLength = ceilToPowerOf2(length);
  if (static_cast<float>(length) / ceiledLength > 0.95f) {
    return ceiledLength * 2;
  } else {
    return ceiledLength;
  }
}

void packRects(std::vector<stbrp_rect> &rects, stbrp_coord length) {
  std::vector<stbrp_node> nodes(length);
  
  int success;
  int retries = 0;
  do {
    if (retries) {
      std::cout << "Rectangle pack failed. Retrying with double size\n";
    }
    if (retries == 4) {
      throw RectPackError();
    }
  
    stbrp_context ctx;
    stbrp_init_target(&ctx, length, length, nodes.data(), static_cast<int>(nodes.size()));
    success = stbrp_pack_rects(&ctx, rects.data(), static_cast<int>(rects.size()));
    
    length *= 2;
    ++retries;
  } while (success == 0);
}

stbrp_coord packRects(std::vector<stbrp_rect> &rects) {
  std::cout << "Packing rectangles\n";
  
  if (rects.size() == 0) {
    return 0;
  }
  
  const stbrp_coord length = calcLength(calcArea(rects));
  packRects(rects, length);
  
  return length;
}
