//
//  interpolate.hpp
//  Game Engine
//
//  Created by Indi Kernick on 4/08/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_math_interpolate_hpp
#define engine_math_interpolate_hpp

#include <cmath>
#include <type_traits>
#include "constants.hpp"

namespace Math {
  template <typename Float, typename T>
  std::enable_if_t<
    std::is_floating_point<Float>::value &&
    std::is_arithmetic<T>::value,
    T
  >
  lerp(const Float t, const T from, const T to) {
    return from + static_cast<Float>(to - from) * t;
  }
  
  template <typename Float, typename T>
  std::enable_if_t<
    std::is_floating_point<Float>::value &&
    std::is_arithmetic<T>::value,
    Float
  >
  invLerp(const T value, const T from, const T to) {
    return static_cast<Float>(value - from) / static_cast<Float>(to - from);
  }
  
  template <typename Float>
  using IsFloat = std::enable_if_t<std::is_floating_point<Float>::value, Float>;
  
  ///Repetitions are normalized
  template <typename Float>
  IsFloat<Float> norm(const Float t) {
    return std::fmod(Float(1) - std::fmod(-t, Float(1)), Float(1));
  }
  
  ///Odd repetitions are mirrored
  template <typename Float>
  IsFloat<Float> normMirror(const Float t) {
    const Float normT = norm(t);
    return std::fmod(std::floor(t), Float(2)) ? Float(1) - normT : normT;
  }
  
  ///Uses the -PId2 to 0 range of the sin function
  template <typename Float>
  IsFloat<Float> sinIn(const Float t) {
    return Float(1) + std::sin(PId2 * t - PId2);
  }
  ///Uses the 0 to PId2 range of the sin function
  template <typename Float>
  IsFloat<Float> sinOut(const Float t) {
    return std::sin(PId2 * t);
  }
  ///Uses the -PId2 to PId2 range of the sin function
  template <typename Float>
  IsFloat<Float> sinInOut(const Float t) {
    return (Float(1) + std::sin(PI * t - PId2)) / Float(2);
  }
  
  ///Uses the -2 to 0 range of the erf function
  template <typename Float>
  IsFloat<Float> errorIn(const Float t) {
    return Float(1) + std::erf(Float(2) * t - Float(2));
  }
  ///Uses the 0 to 2 range of the erf function
  template <typename Float>
  IsFloat<Float> errorOut(const Float t) {
    return std::erf(Float(2) * t);
  }
  ///Uses the -2 to 2 range of the erf function
  template <typename Float>
  IsFloat<Float> errorInOut(const Float t) {
    return (Float(1) + std::erf(Float(4) * t - Float(2))) / Float(2);
  }
  
  /*
  Thank you so much Gre for easing.js
  
  https://gist.github.com/gre/1650294
  */
  
  template <typename Float>
  IsFloat<Float> quadIn(const Float t) {
    return t*t;
  }
  template <typename Float>
  IsFloat<Float> quadOut(const Float t) {
    return t * (Float(2) - t);
  }
  template <typename Float>
  IsFloat<Float> quadInOut(const Float t) {
    return t < Float(0.5)
           ? Float(2) * t*t
           : Float(-1) + (Float(4) - Float(2) * t) * t;
  }
  
  template <typename Float>
  IsFloat<Float> cubicIn(const Float t) {
    return t*t*t;
  }
  template <typename Float>
  IsFloat<Float> cubicOut(const Float t) {
    const Float tm1 = t - Float(1);
    return tm1*tm1*tm1 + Float(1);
  }
  template <typename Float>
  IsFloat<Float> cubicInOut(const Float t) {
    return t < Float(0.5)
           ? Float(4) * t*t*t
           : (t - Float(1)) * (Float(2) * t - Float(2)) * (Float(2) * t - Float(2)) + Float(1);
  }
  
  template <typename Float>
  IsFloat<Float> quartIn(const Float t) {
    return t*t*t*t;
  }
  template <typename Float>
  IsFloat<Float> quartOut(const Float t) {
    const Float tm1 = t - Float(1);
    return Float(1) - tm1*tm1*tm1*tm1;
  }
  template <typename Float>
  IsFloat<Float> quartInOut(const Float t) {
    const Float tm1 = t - Float(1);
    return t < Float(0.5)
           ? Float(8) * t*t*t*t
           : Float(1) - Float(8) * tm1*tm1*tm1*tm1;
  }
  
  template <typename Float>
  IsFloat<Float> quintIn(const Float t) {
    return t*t*t*t*t;
  }
  template <typename Float>
  IsFloat<Float> quintOut(const Float t) {
    const Float tm1 = t - Float(1);
    return Float(1) + tm1*tm1*tm1*tm1*tm1;
  }
  template <typename Float>
  IsFloat<Float> quintInOut(const Float t) {
    const Float tm1 = t - Float(1);
    return t < Float(0.5)
           ? Float(16) * t*t*t*t*t
           : Float(1) + Float(16) * tm1*tm1*tm1*tm1*tm1;
  }
};

#endif
