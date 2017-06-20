//
//  input manager.hpp
//  Game Engine
//
//  Created by Indi Kernick on 3/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_input_manager_hpp
#define engine_platform_input_manager_hpp

#include <SDL2/SDL_events.h>
#include "window manager.hpp"
#include "../Input/event.hpp"
#include "../Event/manager.hpp"

namespace Platform {
  class InputManager {
  public:
    using Ptr = std::shared_ptr<InputManager>;
  
    explicit InputManager(std::weak_ptr<WindowManager>);
    ~InputManager() = default;
    
    void update();
    
  private:
    std::weak_ptr<WindowManager> weakWindowManager;
    bool keyState[Input::Key::NUM_OF_KEYS] = {0};
    bool mouseState[Input::MButton::NUM_OF_BUTTONS] = {0};
    glm::ivec2 mousePos;
  
    static Input::Key::Type fromScancode(int);
    static Input::MButton::Type fromIndex(uint8_t);
    
    void sendMouseDown(const SDL_MouseButtonEvent &, WindowManager::Ptr);
    void sendMouseUp(const SDL_MouseButtonEvent &, WindowManager::Ptr);
    void sendMouseMove(const SDL_MouseMotionEvent &, WindowManager::Ptr);
    void sendScroll(const SDL_MouseWheelEvent &, WindowManager::Ptr);
    void sendKeyDown(const SDL_KeyboardEvent &, WindowManager::Ptr);
    void sendKeyUp(const SDL_KeyboardEvent &, WindowManager::Ptr);
    void sendWindowEvent(const SDL_WindowEvent &, WindowManager::Ptr);
    void sendQuit(WindowManager::Ptr);
  };
}

#endif
