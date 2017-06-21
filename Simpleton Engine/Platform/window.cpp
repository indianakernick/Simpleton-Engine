//
//  window.cpp
//  Game Engine
//
//  Created by Indi Kernick on 2/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "window.hpp"

using namespace Platform;

Window::Window(SDL_Window *window)
  : ImplicitUniquePtr(window) {
  assert(window);
}

Window::ID Window::getID() const {
  return SDL_GetWindowID(ptr);
}

void Window::title(const std::string &newTitle) {
  SDL_SetWindowTitle(ptr, newTitle.c_str());
}

std::string Window::title() const {
  return SDL_GetWindowTitle(ptr);
}

void Window::pos(const glm::ivec2 newPos) {
  SDL_SetWindowPosition(ptr, newPos.x, newPos.y);
}

glm::ivec2 Window::pos() const {
  glm::ivec2 pos;
  SDL_GetWindowPosition(ptr, &pos.x, &pos.y);
  return pos;
}

void Window::center() {
  SDL_SetWindowPosition(ptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Window::center(const bool x, const bool y) {
  SDL_SetWindowPosition(
    ptr,
    Utils::boolEnable(x, SDL_WINDOWPOS_CENTERED),
    Utils::boolEnable(y, SDL_WINDOWPOS_CENTERED)
  );
}

void Window::size(glm::ivec2 newSize) {
  SDL_SetWindowSize(ptr, newSize.x, newSize.y);
}

glm::ivec2 Window::size() const {
  glm::ivec2 size;
  SDL_GetWindowSize(ptr, &size.x, &size.y);
  return size;
}

void Window::opacity(const float newOpacity) {
  SDL_SetWindowOpacity(ptr, newOpacity);
}

float Window::opacity() const {
  float opacity;
  SDL_GetWindowOpacity(ptr, &opacity);
  return opacity;
}

void Window::relMouse(const bool status) {
  assert(SDL_GetMouseFocus() == ptr);
  SDL_SetRelativeMouseMode(static_cast<SDL_bool>(status));
}

bool Window::relMouse() const {
  return SDL_GetRelativeMouseMode() && SDL_GetMouseFocus() == ptr;
}

void Window::captureMouse(const bool status) {
  assert(SDL_GetMouseFocus() == ptr);
  SDL_CaptureMouse(static_cast<SDL_bool>(status));
  mouseCaptured = status;
}

bool Window::captureMouse() const {
  return mouseCaptured;
}

void Window::raise() {
  SDL_RaiseWindow(ptr);
}
