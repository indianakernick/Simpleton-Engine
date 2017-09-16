//
//  file io.hpp
//  Game Engine
//
//  Created by Indi Kernick on 20/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_file_io_hpp
#define engine_memory_file_io_hpp

//I'll never need this but it was fun to write!

#include <cstdio>
#include <iostream>
#include "buffer.hpp"

namespace Memory {
  class FileError final : public std::runtime_error {
  public:
    explicit FileError(const char *);
  };

  Buffer readFile(const std::string &);
  Buffer readFile(const char *);
  Buffer readFile(std::FILE *);
  Buffer readFile(std::istream &);
  
  void writeFile(const Buffer &, const std::string &);
  void writeFile(const Buffer &, const char *);
  void writeFile(const Buffer &, std::FILE *);
  void writeFile(const Buffer &, std::ostream &);
}

#endif
