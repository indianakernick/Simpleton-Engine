//
//  exceptions.cpp
//  Game Engine
//
//  Created by Indi Kernick on 10/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "exceptions.hpp"

Memory::OutOfMemory::OutOfMemory(const std::string &what)
  : std::runtime_error(what) {}

Memory::OverRun::OverRun(const std::string &what)
  : std::runtime_error(what) {}

Memory::UnderRun::UnderRun(const std::string &what)
  : std::runtime_error(what) {}

Memory::Leak::Leak(const std::string &what)
  : std::runtime_error(what) {}

Memory::BadPtr::BadPtr(const std::string &what)
  : std::runtime_error(what) {}
