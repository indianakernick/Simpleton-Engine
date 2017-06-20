//
//  exceptions.hpp
//  Game Engine
//
//  Created by Indi Kernick on 10/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_exceptions_hpp
#define engine_memory_exceptions_hpp

#include <stdexcept>
#include <string>

namespace Memory {
  class OutOfMemory final : public std::runtime_error {
  public:
    explicit OutOfMemory(const std::string &what);
  };
  
  class OverRun final : public std::runtime_error {
  public:
    explicit OverRun(const std::string &what);
  };
  
  class UnderRun final : public std::runtime_error {
  public:
    explicit UnderRun(const std::string &what);
  };
  
  class Leak final : public std::runtime_error {
  public:
    explicit Leak(const std::string &what);
  };
  
  class BadPtr final : public std::runtime_error {
  public:
    explicit BadPtr(const std::string &what);
  };
}


#endif
