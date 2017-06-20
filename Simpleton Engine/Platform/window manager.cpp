//
//  window manager.cpp
//  Game Engine
//
//  Created by Indi Kernick on 18/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "window manager.hpp"

using namespace Platform;

WindowOpenError::WindowOpenError(const char *what)
  : std::runtime_error(what) {}

std::weak_ptr<Window> WindowManager::openWindow(const Window::Desc &desc) {
  SDL_Window *windowHandle = SDL_CreateWindow(
    desc.title.c_str(),
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    desc.size.x, desc.size.y,
    SDL_WINDOW_SHOWN |
    Utils::boolEnable(desc.resizable, SDL_WINDOW_RESIZABLE) |
    Utils::boolEnable(desc.openGL, SDL_WINDOW_OPENGL)
  );
  
  if (windowHandle == nullptr) {
    throw WindowOpenError(SDL_GetError());
  }
  
  const Window::Ptr window = std::make_shared<Window>(windowHandle);
  windows.emplace(SDL_GetWindowID(windowHandle), window);
  
  return window;
}

void WindowManager::closeWindow(std::weak_ptr<Window> window) {
  if (window.expired()) {
    //Tried to close window that was already closed
    return;
  }
  const Window::Ptr strongWindow = window.lock();
  
  for (auto w = windows.cbegin(); w != windows.cend(); ++w) {
    if (w->second == strongWindow) {
      windows.erase(w);
      return;
    }
  }
  
  //Tried to close window that was already closed
}

void WindowManager::closeAllWindows() {
  windows.clear();
}

std::weak_ptr<Window> WindowManager::getWindow(const WindowID id) {
  const auto iter = windows.find(id);
  assert(iter != windows.end());
  return iter->second;
}

void WindowManager::closeWindow(const WindowID id) {
  const auto iter = windows.find(id);
  assert(iter != windows.end());
  windows.erase(iter);
}
