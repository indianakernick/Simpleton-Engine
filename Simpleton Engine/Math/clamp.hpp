//
//  clamp.hpp
//  Game Engine
//
//  Created by Indi Kernick on 12/09/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_math_clamp_hpp
#define engine_math_clamp_hpp

#include <type_traits>

namespace Math {
  template <typename T>
  constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>
  min(T a, T b) {
    return a < b ? a : b;
  }
  
  template <typename T>
  constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>
  max(T a, T b) {
    return a > b ? a : b;
  }
  
  template <typename T>
  constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>
  clamp(T value, T low, T high) {
    return value > low ? (value < high ? value : high) : low;
  }
  
  template <typename T>
  constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>
  clampMin(T value, T low) {
    return value > low ? value : low;
  }
  
  template <typename T>
  constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>
  clampMax(T value, T high) {
    return value < high ? value : high;
  }
}

#endif
