//
//  vectors.hpp
//  Game Engine
//
//  Created by Indi Kernick on 4/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_math_vectors_hpp
#define engine_math_vectors_hpp

#include <cmath>
#include <glm/vec2.hpp>

namespace Math {
  ///Calculate the aspect ratio of a 2D vector
  template <typename Return, typename Argument>
  Return aspectRatio(const glm::tvec2<Argument> size) {
    return static_cast<Return>(size.x) / static_cast<Return>(size.y);
  }
  
  ///Calculate the squared distance between 2D int vectors
  template <typename Int>
  Int distance2(const glm::tvec2<Int> a, const glm::tvec2<Int> b) {
    static_assert(
      std::is_integral<Int>::value,
      "glm/gtx/norm.hpp provides distance2 for float vectors"
    );
    
    const Int x = a.x - b.x;
    const Int y = a.y - b.y;
    return x*x + y*y;
  }
  
  ///Get a vector from an angle and a magnitude
  template <typename Return, typename Angle, typename Mag>
  glm::tvec2<Return> angleMag(const Angle angle, const Mag mag) {
    return {
      static_cast<Return>(std::cos(angle) * mag),
      static_cast<Return>(std::sin(angle) * mag)
    };
  }
  
  ///Get a vector from an angle and a magnitude
  template <typename Angle, typename Mag>
  glm::tvec2<Mag> angleMag(const Angle angle, const Mag mag) {
    return angleMag<Mag, Angle, Mag>(angle, mag);
  }
}

#endif
