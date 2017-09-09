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
#include "sdl error.hpp"
#include "../Utils/instance limiter.hpp"

namespace Platform {
  class WindowLibrary final : public Utils::ForceSingleton<WindowLibrary> {
  public:
    WindowLibrary();
    explicit WindowLibrary(Uint32);
    ~WindowLibrary();
  };
  
  Window makeWindow(const Window::Desc &);
  Renderer makeRenderer(Window &, bool);
}

#endif
