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
  template<typename T>
  inline std::enable_if_t<std::is_arithmetic<T>::value, T>
  scale(T value, T fromMin, T fromMax, T toMin, T toMax) {
    return (value / (fromMax - fromMin) - fromMin) * (toMax - toMin) + toMin;
  }
  ///Scales a value from its range to a range of 0-1
  template<typename T>
  inline std::enable_if_t<std::is_arithmetic<T>::value, T>
  fromRange(T value, T min, T max) {
    return (value - min) / (max - min);
  }
  ///Scales a value from a range of 0-1 to a new range
  template<typename T>
  inline std::enable_if_t<std::is_arithmetic<T>::value, T>
  toRange(T value, T min, T max) {
    return value * (max - min) + min;
  }
}

#endif
