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

#include "buffer.hpp"
#include <cstdio>
#include <iostream>

namespace Memory {
  class FileError final : public std::runtime_error {
  public:
    explicit FileError(const char *);
  };

  Memory::Buffer readFile(const std::string &);
  Memory::Buffer readFile(const char *);
  Memory::Buffer readFile(std::FILE *);
  Memory::Buffer readFile(std::istream &);
  
  void writeFile(const Memory::Buffer &, const std::string &);
  void writeFile(const Memory::Buffer &, const char *);
  void writeFile(const Memory::Buffer &, std::FILE *);
  void writeFile(const Memory::Buffer &, std::ostream &);
}

#endif
