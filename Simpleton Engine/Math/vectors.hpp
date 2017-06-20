//
//  vectors.hpp
//  Game Engine
//
//  Created by Indi Kernick on 4/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_math_vectors_hpp
#define engine_math_vectors_hpp

#include <glm/vec2.hpp>

namespace Math {
  ///Calculate the aspect ratio of a 2D vector
  template <typename RET, typename ARG>
  inline RET aspectRatio(glm::tvec2<ARG> size) {
    return static_cast<RET>(size.x) / static_cast<RET>(size.y);
  }
}

#endif
