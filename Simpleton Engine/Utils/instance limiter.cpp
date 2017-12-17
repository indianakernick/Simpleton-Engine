//
//  instance limiter.cpp
//  Game Engine
//
//  Created by Indi Kernick on 9/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "instance limiter.hpp"

#include <string>

Utils::TooManyInstances::TooManyInstances(
  const std::string_view type,
  const size_t maxCount
) : std::logic_error(
      "Too many instances of type \"" +
      std::string(type) +
      "\"\nMaximum is " +
      std::to_string(maxCount)
    ) {}
