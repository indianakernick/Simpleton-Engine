//
//  write atlas.cpp
//  Pack
//
//  Created by Indi Kernick on 24/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include "write atlas.hpp"

#include <fstream>
#include <Simpleton/Utils/profiler.hpp>

std::string_view getImageName(const std::string &path) {
  const size_t lastSlash = path.find_last_of('/');
  return {path.c_str() + lastSlash + 1, path.find_last_of('.') - lastSlash - 1};
}

void writeName(std::ofstream &file, const std::string &path, const size_t index) {
  file << '"' << getImageName(path) << "\":" << index;
}

void writeRect(std::ofstream &file, stbrp_rect rect, const stbrp_coord sep) {
  rect.w = rect.w - sep + rect.x;
  rect.h = rect.h - sep + rect.y;
  std::swap(rect.h, rect.y);
  file << '['
    << static_cast<float>(rect.x) << ','
    << static_cast<float>(rect.y) << ','
    << static_cast<float>(rect.w) << ','
    << static_cast<float>(rect.h) << ']'
  ;
}

void writeAtlas(
  const std::string_view path,
  const std::vector<std::string> &names,
  const std::vector<stbrp_rect> &rects,
  const stbrp_coord length,
  const stbrp_coord sep
) {
  PROFILE(writeAtlas);
  
  if (length > 16777216) {
    // 2^24 = 16777216
    // a single precision float has 24 mantissa bits
    // a single precision float can accuratly store n/16777216
    // 16777217.0f == 16777216.0f 
    throw std::runtime_error("Texture size is too large");
  }
  
  std::ofstream file{std::string(path)};
  file << "{\"length\":" << length << ",\"names\":{";
  for (size_t n = 0; n != names.size() - 1; ++n) {
    writeName(file, names[n], n);
    file << ',';
  }
  writeName(file, names.back(), names.size() - 1);
  
  file << "},\"rects\":[";
  
  for (auto r = rects.cbegin(); r != rects.cend() - 1; ++r) {
    writeRect(file, *r, sep);
    file << ',';
  }
  writeRect(file, rects.back(), sep);
  
  file << "]}\n";
}
