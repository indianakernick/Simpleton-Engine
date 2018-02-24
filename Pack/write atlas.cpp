//
//  write atlas.cpp
//  Pack
//
//  Created by Indi Kernick on 24/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include "write atlas.hpp"

#include <Simpleton/Memory/file io.hpp>
#include <Simpleton/Utils/compose string.hpp>

void writeAtlas(
  const std::string_view path,
  const std::vector<std::string> &names,
  const std::vector<stbrp_rect> &rects,
  const stbrp_coord length,
  const stbrp_coord sep
) {
  if (length > 16777216) {
    // 2^24 = 16777216
    // a single precision float has 24 mantissa bits
    // a single precision float can accuratly store n/16777216
    // 16777217.0f == 16777216.0f 
    throw std::runtime_error("Texture size is too large");
  }
  
  Utils::ComposeString string(names.size() * 32 + rects.size() * 32);
  string.writeNumberLil(static_cast<uint32_t>(names.size()));
  for (auto n = names.cbegin(); n != names.cend(); ++n) {
    string.write(*n);
    string.write('\n');
  }
  for (auto r = rects.cbegin(); r != rects.cend(); ++r) {
    stbrp_rect rect = *r;
    rect.w -= sep;
    rect.h -= sep;
    rect.y = length - rect.y - rect.h;
    rect.w += rect.x;
    rect.h += rect.y;
    string.writeNumberLil(static_cast<float>(rect.x) / length);
    string.writeNumberLil(static_cast<float>(rect.y) / length);
    string.writeNumberLil(static_cast<float>(rect.w) / length);
    string.writeNumberLil(static_cast<float>(rect.h) / length);
  }
  
  const std::string_view view = string.view();
  const Memory::FileHandle file = Memory::openFileWrite(path.data());
  Memory::writeFile(view.data(), view.size(), file.get());
}
