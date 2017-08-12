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
  min(const T a, const T b) {
    return a < b ? a : b;
  }
  
  template <typename T>
  constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>
  max(const T a, const T b) {
    return a > b ? a : b;
  }
  
  template <typename T>
  constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>
  clamp(const T value, const T low, const T high) {
    return value > low ? (value < high ? value : high) : low;
  }
  
  template <typename T>
  constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>
  clampMin(const T value, const T low) {
    return value > low ? value : low;
  }
  
  template <typename T>
  constexpr std::enable_if_t<std::is_arithmetic<T>::value, T>
  clampMax(const T value, const T high) {
    return value < high ? value : high;
  }
}

#endif
