//
//  window library.hpp
//  Game Engine
//
//  Created by Indi Kernick on 3/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_window_library_hpp
#define engine_platform_window_library_hpp

#include <SDL2/SDL.h>
#include "window.hpp"
#include "renderer.hpp"

namespace Platform {
  class LibInitError : public std::runtime_error {
  public:
    explicit LibInitError(const char *);
  };

  void initLib();
  void quitLib();
  
  Renderer::Ptr makeRenderer(const Window::Ptr, bool);
}

#endif
