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
#include "../Utils/instance limiter.hpp"

namespace Platform {
  class WindowLibInitError : public std::runtime_error {
  public:
    explicit WindowLibInitError(const char *);
  };
  
  class WindowOpenError : public std::runtime_error {
  public:
    explicit WindowOpenError(const char *);
  };
  
  class RendererCreateError : public std::runtime_error {
  public:
    explicit RendererCreateError(const char *);
  };

  class WindowLibrary final : public Utils::ForceSingleton<WindowLibrary> {
  public:
    WindowLibrary();
    ~WindowLibrary();
    
    Window makeWindow(const Window::Desc &);
    Renderer makeRenderer(Window &, bool);
  };
}

#endif
