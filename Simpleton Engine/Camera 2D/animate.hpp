//
//  animate.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 26/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef animate_hpp
#define animate_hpp

#include "props.hpp"

namespace Cam2D {
  template <PropID PROP>
  class Animate {
  public:
    using Type = PropType<PROP>;
  
    Animate() = default;
    virtual ~Animate() = default;
    
    virtual Type calculate(Props, Type target, float delta) = 0;
  };
}

#endif
