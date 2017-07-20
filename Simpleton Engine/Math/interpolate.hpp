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
  template<typename T>
  inline std::enable_if_t<std::is_arithmetic<T>::value, T>
  lerp(double t, T from, T to) {
    return from + (to - from) * t;
  }
  
  template<typename T>
  inline std::enable_if_t<std::is_arithmetic<T>::value, double>
  invLerp(T value, T from, T to) {
    return (value - from) / (to - from);
  }
  
  ///Repetitions are normalized
  inline double norm(double t) {
    return std::fmod(1.0 - std::fmod(-t, 1.0), 1.0);
  }
  
  ///Odd repetitions are mirrored
  inline double normMirror(double t) {
    //there's only one call to norm so the function is smaller
    //because norm is inline
    const double normT = norm(t);
    return std::fmod(std::floor(t), 2.0) ? 1.0 - normT : normT;
  }
  
  ///Uses the -PId2 to 0 range of the sin function
  inline double sinIn(double t) {
    return 1 + std::sin(PId2 * t - PId2);
  }
  ///Uses the 0 to PId2 range of the sin function
  inline double sinOut(double t) {
    return std::sin(PId2 * t);
  }
  ///Uses the -PId2 to PId2 range of the sin function
  inline double sinInOut(double t) {
    return (1 + std::sin(PI * t - PId2)) / 2;
  }
  
  ///Uses the -2 to 0 range of the erf function
  inline double errorIn(double t) {
    return 1 + std::erf(2 * t - 2);
  }
  ///Uses the 0 to 2 range of the erf function
  inline double errorOut(double t) {
    return std::erf(2 * t);
  }
  ///Uses the -2 to 2 range of the erf function
  inline double errorInOut(double t) {
    return (1 + std::erf(4 * t - 2)) / 2;
  }
  
  /*
  Thank you so much Gre for easing.js
  
  https://gist.github.com/gre/1650294
  */
  
  inline double quadIn(double t) {
    return t*t;
  }
  inline double quadOut(double t) {
    return t * (2 - t);
  }
  inline double quadInOut(double t) {
    return t < 0.5 ? 2 * t*t
                   : -1 + (4 - 2 * t) * t;
  }
  
  inline double cubicIn(double t) {
    return t*t*t;
  }
  inline double cubicOut(double t) {
    const double tm1 = t - 1;
    return tm1*tm1*tm1 + 1;
  }
  inline double cubicInOut(double t) {
    return t < 0.5 ? 4 * t*t*t
                   : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
  }
  
  inline double quartIn(double t) {
    return t*t*t*t;
  }
  inline double quartOut(double t) {
    const double tm1 = t - 1;
    return 1 - tm1*tm1*tm1*tm1;
  }
  inline double quartInOut(double t) {
    const double tm1 = t - 1;
    return t < 0.5 ? 8 * t*t*t*t
                   : 1 - 8 * tm1*tm1*tm1*tm1;
  }
  
  inline double quintIn(double t) {
    return t*t*t*t*t;
  }
  inline double quintOut(double t) {
    const double tm1 = t - 1;
    return 1 + tm1*tm1*tm1*tm1*tm1;
  }
  inline double quintInOut(double t) {
    const double tm1 = t - 1;
    return t < 0.5 ? 16 * t*t*t*t*t
                   : 1 + 16 * tm1*tm1*tm1*tm1*tm1;
  }
};

#endif
