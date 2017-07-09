//
//  instance limiter.cpp
//  Game Engine
//
//  Created by Indi Kernick on 9/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "instance limiter.hpp"

Utils::TooManyInstances::TooManyInstances(
  const std::experimental::string_view type,
  const size_t maxCount
) : std::logic_error(
      "Too many instances of type \"" +
      type.to_string() +
      "\"\nMaximum is " +
      std::to_string(maxCount)
    ) {}
