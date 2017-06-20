//
//  mouse.hpp
//  Game Engine
//
//  Created by Indi Kernick on 5/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_input_mouse_hpp
#define engine_input_mouse_hpp

#include <cstdint>

namespace Input {
  namespace MButton {
    using Type = uint8_t;
    enum : Type {
      UNKNOWN,
      LEFT,
      MIDDLE,
      RIGHT,
      NUM_OF_BUTTONS
    };
  }
}

#endif
