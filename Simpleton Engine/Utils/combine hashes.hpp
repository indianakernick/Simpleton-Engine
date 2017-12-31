//
//  combine hashes.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 28/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_combine_hashes_hpp
#define engine_utils_combine_hashes_hpp

#include <cstddef>

namespace Utils {
  constexpr size_t combineHashes(const size_t left, const size_t right) {
    static_assert(sizeof(size_t) == 8, "Only 64 bit hashes can be combined");
  
    //https://stackoverflow.com/a/8980550
    
    const size_t mul = 0x9E3779B97F4A7C15ull;
    size_t a = (right ^ left) * mul;
    a ^= (a >> 47);
    size_t b = (left ^ a) * mul;
    b ^= (b >> 47);
    return b * mul;
  }
}

#endif
