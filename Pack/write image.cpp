//
//  write image.cpp
//  Pack
//
//  Created by Indi Kernick on 24/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include "write image.hpp"

#include <iostream>
#include <libpng16/png.h>
#include <Simpleton/Utils/profiler.hpp>


void writeImage(const std::string &path, G2D::Surface image) {
  PROFILE(writeImage);
  std::cout << "Writing image to file \"" << path << "\"\n";
  
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (png == nullptr) {
    std::cerr << "Error creating png_struct\n";
    return;
  }
  
  png_infop info = png_create_info_struct(png);
  if (info == nullptr) {
    std::cerr << "Error creating png_info\n";
    png_destroy_write_struct(&png, nullptr);
    return;
  }
  
  if (setjmp(png_jmpbuf(png))) {
    std::cerr << "Error setting jump buf\n";
    png_destroy_write_struct(&png, &info);
    return;
  }
  
  FILE *file = fopen(path.c_str(), "wb");
  if (file == nullptr) {
    std::cerr << "Error opening file\n";
    png_destroy_write_struct(&png, &info);
    return;
  }
  
  png_init_io(png, file);
  
  png_set_IHDR(
    png,
    info,
    image.width(),
    image.height(),
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  
  png_write_info(png, info);
  
  std::vector<png_bytep> rows;
  rows.reserve(image.height());
  for (auto row = image.data(); row != image.dataEnd(); row += image.pitch()) {
    rows.push_back(row);
  }
  
  png_write_image(png, rows.data());
  png_write_end(png, nullptr);
  
  png_destroy_write_struct(&png, &info);
  fclose(file);
}
