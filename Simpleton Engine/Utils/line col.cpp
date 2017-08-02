//
//  line col.cpp
//  Game Engine
//
//  Created by Indi Kernick on 25/2/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "line col.hpp"

Utils::LineColRangeError::LineColRangeError()
  : std::range_error("Line or column out of range") {}
