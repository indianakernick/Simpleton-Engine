//
//  app.cpp
//  Pack
//
//  Created by Indi Kernick on 24/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include "app.hpp"

#include <iostream>
#include "search dir.hpp"
#include "sort by frame.hpp"
#include "load images.hpp"
#include "rects from images.hpp"
#include "pack rects.hpp"
#include "blit images.hpp"
#include "write atlas.hpp"
#include <Simpleton/Graphics 2D/write surface.hpp>

void printUsage() {
  std::cout <<
R"(pack [in=<in>] [out=<out>] [sep=<sep>] [white=<white>] [rec=<rec>]
  
  in     Input directory to search for images                        [default=.]
  out    Output file name without extension                    [default=sprites]
  sep    Number of pixels separating each image                      [default=1]
  white  Radius of the whitepixel                                    [default=0]
  rec    Maximum recursive search depth                              [default=1]
)";
}

unsigned long parseInt(const char *str) {
  // std::from_chars should have been in C++ since 1998
  // IM STILL WAITING
  
  char *end;
  const unsigned long n = std::strtoul(str, &end, 10);
  if (n == 0 && end[-1] != '0') {
    throw std::runtime_error("Invalid integer");
  }
  return n;
}

void runApp(int argc, const char **argv) {
  std::string in = ".";
  std::string out = "sprites";
  stbrp_coord sep = 1;
  stbrp_coord white = 0;
  size_t rec = 1;
  
  const char **const end = argv + argc;
  for (; argv != end; ++argv) {
    if (std::strncmp(*argv, "in", 2) == 0) {
      if (argv[0][2] != '=' || argv[0][3] == 0) {
        throw ArgError();
      }
      in.clear();
      in.append(argv[0] + 3);
    } else if (std::strncmp(*argv, "out", 3) == 0) {
      if (argv[0][3] != '=' || argv[0][4] == 0) {
        throw ArgError();
      }
      out.clear();
      out.append(argv[0] + 4);
    } else if (std::strncmp(*argv, "sep", 3) == 0) {
      if (argv[0][3] != '=' || argv[0][4] == 0) {
        throw ArgError();
      }
      sep = static_cast<stbrp_coord>(parseInt(argv[0] + 4));
    } else if (std::strncmp(*argv, "white", 5) == 0) {
      if (argv[0][5] != '=' || argv[0][6] == 0) {
        throw ArgError();
      }
      white = static_cast<stbrp_coord>(parseInt(argv[0] + 6));
    } else if (std::strncmp(*argv, "rec", 3) == 0) {
      if (argv[0][3] != '=' || argv[0][4] == 0) {
        throw ArgError();
      }
      rec = static_cast<size_t>(parseInt(argv[0] + 4));
    }
  }
  
  std::remove((out + ".png").c_str());
  std::vector<std::string> paths = findFiles(in, extIsImage, rec);
  sortByFrame(paths);
  
  std::vector<G2D::Surface> images = loadImages(paths);
  if (white) {
    const stbrp_coord size = 1 + white * 2;
    images.emplace_back(size, size, 4, 255);
    paths.emplace_back("__WHITEPIXEL__.png");
  }
  
  std::vector<stbrp_rect> rects = rectsFromImages(images, sep);
  const stbrp_coord length = packRects(rects);
  
  const G2D::Surface image = blitImages(images, rects, length);
  std::cout << "Writing image to file \"" << out << ".png\"\n";
  G2D::writeSurface(out + ".png", image);
  std::cout << "Writing atlas to file \"" << out << ".atlas\"\n";
  writeAtlas(out + ".atlas", paths, rects, length, sep);
}
