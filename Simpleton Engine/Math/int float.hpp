//
//  int float.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 26/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_math_int_float_hpp
#define engine_math_int_float_hpp

#include <cmath>
#include <limits>
#include <type_traits>

namespace Math {
  template <typename LeftNumber, typename RightNumber>
  constexpr std::common_type_t<LeftNumber, RightNumber>
  mod(const LeftNumber left, const RightNumber right) {
    using Ret = std::common_type_t<LeftNumber, RightNumber>;
    if constexpr (std::is_floating_point<Ret>::value) {
      return std::fmod(left, right);
    } else {
      return left % right;
    }
  }
  
  template <typename Number>
  constexpr Number infinity() {
    using Limits = std::numeric_limits<Number>;
    if constexpr (Limits::has_infinity()) {
      return Limits::infinity();
    } else {
      return Limits::max();
    }
  }
  
  template <typename Number>
  constexpr Number negInfinity() {
    using Limits = std::numeric_limits<Number>;
    if constexpr (Limits::has_infinity()) {
      return -Limits::infinity();
    } else {
      return Limits::lowest();
    }
  }
}

#endif
