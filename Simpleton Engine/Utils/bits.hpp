//
//  bits.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 12/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_bits_hpp
#define engine_utils_bits_hpp

#include <climits>
#include <cstddef>

namespace Utils {
  ///Size of a type in bits
  template <typename T>
  constexpr size_t bits = sizeof(T) * CHAR_BIT;
}

#endif
