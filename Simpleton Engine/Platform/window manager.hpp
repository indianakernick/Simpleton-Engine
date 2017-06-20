//
//  window manager.hpp
//  Game Engine
//
//  Created by Indi Kernick on 18/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_sdl_window_manager_hpp
#define engine_platform_sdl_window_manager_hpp

#include "window.hpp"
#include <iostream>
#include <unordered_map>
#include "../Utils/bool enable.hpp"

namespace Platform {
  class WindowOpenError final : std::runtime_error {
  public:
    explicit WindowOpenError(const char *);
  };

  class WindowManager {
  public:
    using Ptr = std::shared_ptr<WindowManager>;
    using WindowID = Uint32;
    
    WindowManager() = default;
    ~WindowManager() = default;
    
    std::weak_ptr<Window> openWindow(const Window::Desc &);
    void closeWindow(std::weak_ptr<Window>);
    void closeAllWindows();
    std::weak_ptr<Window> getWindow(WindowID);
    void closeWindow(WindowID);
  
  private:
    std::unordered_map<WindowID, Window::Ptr> windows;
  };
}

#endif
