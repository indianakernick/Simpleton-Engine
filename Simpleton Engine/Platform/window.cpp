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
  : window(window, &SDL_DestroyWindow) {
  assert(window);
}

Window::ID Window::getID() const {
  return SDL_GetWindowID(window.get());
}

void Window::title(const std::string &newTitle) {
  SDL_SetWindowTitle(window.get(), newTitle.c_str());
}

std::string Window::title() const {
  return SDL_GetWindowTitle(window.get());
}

void Window::pos(const glm::ivec2 newPos) {
  SDL_SetWindowPosition(window.get(), newPos.x, newPos.y);
}

glm::ivec2 Window::pos() const {
  glm::ivec2 pos;
  SDL_GetWindowPosition(window.get(), &pos.x, &pos.y);
  return pos;
}

void Window::center() {
  SDL_SetWindowPosition(window.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Window::center(const bool x, const bool y) {
  SDL_SetWindowPosition(
    window.get(),
    Utils::boolEnable(x, SDL_WINDOWPOS_CENTERED),
    Utils::boolEnable(y, SDL_WINDOWPOS_CENTERED)
  );
}

void Window::size(glm::ivec2 newSize) {
  SDL_SetWindowSize(window.get(), newSize.x, newSize.y);
}

glm::ivec2 Window::size() const {
  glm::ivec2 size;
  SDL_GetWindowSize(window.get(), &size.x, &size.y);
  return size;
}

void Window::opacity(const float newOpacity) {
  SDL_SetWindowOpacity(window.get(), newOpacity);
}

float Window::opacity() const {
  float opacity;
  SDL_GetWindowOpacity(window.get(), &opacity);
  return opacity;
}

void Window::relMouse(const bool status) {
  assert(SDL_GetMouseFocus() == window.get());
  SDL_SetRelativeMouseMode(static_cast<SDL_bool>(status));
}

bool Window::relMouse() const {
  return SDL_GetRelativeMouseMode() && SDL_GetMouseFocus() == window.get();
}

void Window::captureMouse(const bool status) {
  assert(SDL_GetMouseFocus() == window.get());
  SDL_CaptureMouse(static_cast<SDL_bool>(status));
  mouseCaptured = status;
}

bool Window::captureMouse() const {
  return mouseCaptured;
}

void Window::raise() {
  SDL_RaiseWindow(window.get());
}

SDL_Window *Window::get() const {
  return window.get();
}

void Window::reset(SDL_Window *newWindow) {
  window.reset(newWindow);
}
