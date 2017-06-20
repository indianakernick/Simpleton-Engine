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
#include "window manager.hpp"
#include "input manager.hpp"
#include "../Utils/instance limiter.hpp"

namespace Platform {
  class LibInitError : public std::runtime_error {
  public:
    explicit LibInitError(const char *);
  };

  class WindowLibrary final : public Utils::ForceSingleton<WindowLibrary> {
  public:
    WindowLibrary();
    ~WindowLibrary();
    
    WindowManager::Ptr makeWindowManager();
    InputManager::Ptr makeInputManager(WindowManager::Ptr);
    Renderer::Ptr makeRenderer(const Window::Ptr, bool);
  };
}

#endif
