//
//  scale.hpp
//  Game Engine
//
//  Created by Indi Kernick on 12/08/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_math_scale_hpp
#define engine_math_scale_hpp

#include <type_traits>

namespace Math {
  ///Scales a value from one range to another
  template <typename T>
  std::enable_if_t<std::is_arithmetic<T>::value, T>
  scale(const T value, const T fromMin, const T fromMax, const T toMin, const T toMax) {
    return (value / (fromMax - fromMin) - fromMin) * (toMax - toMin) + toMin;
  }
  ///Scales a value from its range to a range of 0-1
  template <typename T>
  std::enable_if_t<std::is_arithmetic<T>::value, T>
  fromRange(const T value, const T min, const T max) {
    return (value - min) / (max - min);
  }
  ///Scales a value from a range of 0-1 to a new range
  template <typename T>
  std::enable_if_t<std::is_arithmetic<T>::value, T>
  toRange(const T value, const T min, const T max) {
    return value * (max - min) + min;
  }
  
  template <typename T>
  std::enable_if_t<std::is_unsigned<T>::value, T>
  sign(const T num) {
    return num == T(0) ? T(0) : T(1);
  }
  
  template <typename T>
  std::enable_if_t<
    std::is_signed<T>::value ||
    std::is_floating_point<T>::value,
    T
  >
  sign(const T num) {
    if (num == T(0)) {
      return T(0);
    } else if (num < T(0)) {
      return T(-1);
    } else {
      return T(1);
    }
  }
}

#endif
