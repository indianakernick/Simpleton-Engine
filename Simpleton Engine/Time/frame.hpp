//
//  frame.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 30/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_time_frame_hpp
#define engine_time_frame_hpp

#include <cmath>
#include <type_traits>

namespace Time {
  template <typename Frame, typename Progress>
  std::enable_if_t<
    std::is_integral<Frame>::value &&
    std::is_floating_point<Progress>::value,
    Frame
  >
  progToFrame(const Progress prog, const Frame numFrames) {
    return static_cast<Frame>(prog * static_cast<Progress>(numFrames));
  }
  
  template <typename Frame, typename Progress>
  std::enable_if_t<
    std::is_integral<Frame>::value &&
    std::is_floating_point<Progress>::value,
    Frame
  >
  wrapProgToFrame(const Progress prog, const Frame numFrames) {
    return static_cast<Frame>(prog * static_cast<Progress>(numFrames)) % numFrames;
  }
  
  template <typename Frame, typename Progress>
  std::enable_if_t<
    std::is_integral<Frame>::value &&
    std::is_floating_point<Progress>::value,
    Frame
  >
  progToNearestFrame(const Progress prog, const Frame numFrames) {
    return static_cast<Frame>(std::round(prog * static_cast<Progress>(numFrames)));
  }
  
  template <typename Frame, typename Progress>
  std::enable_if_t<
    std::is_integral<Frame>::value &&
    std::is_floating_point<Progress>::value,
    Frame
  >
  wrapProgToNearestFrame(const Progress prog, const Frame numFrames) {
    return static_cast<Frame>(std::round(prog * static_cast<Progress>(numFrames))) % numFrames;
  }
}

#endif
