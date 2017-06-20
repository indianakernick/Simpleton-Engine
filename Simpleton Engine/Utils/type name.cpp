//
//  type name.cpp
//  Game Engine
//
//  Created by Indi Kernick on 18/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "type name.hpp"

#ifdef __GNUG__

#include <cxxabi.h>
#include <memory>
#include <cstdlib>

std::string Utils::demangle(const char *mangledName) {
  int status;
  std::unique_ptr<char, void (*)(void *)> name {
    abi::__cxa_demangle(mangledName, nullptr, nullptr, &status),
    std::free
  };
  return status == 0 ? name.get() : mangledName;
}

#else

std::string Utils::demangle(const char *mangledName) {
  return mangledName;
}

#endif
