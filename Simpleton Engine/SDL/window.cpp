//
//  window.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 2/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "window.hpp"

#include "sdl error.hpp"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_mouse.h>

using namespace SDL;

Window::ID Window::getID() const {
  return SDL_GetWindowID(window);
}

void Window::title(const std::string &newTitle) {
  SDL_SetWindowTitle(window, newTitle.c_str());
}

std::string Window::title() const {
  return SDL_GetWindowTitle(window);
}

void Window::center() {
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Window::pos(const glm::ivec2 newPos) {
  SDL_SetWindowPosition(window, newPos.x, newPos.y);
}

glm::ivec2 Window::pos() const {
  glm::ivec2 pos;
  SDL_GetWindowPosition(window, &pos.x, &pos.y);
  return pos;
}

void Window::size(glm::ivec2 newSize) {
  SDL_SetWindowSize(window, newSize.x, newSize.y);
}

glm::ivec2 Window::size() const {
  glm::ivec2 size;
  SDL_GetWindowSize(window, &size.x, &size.y);
  return size;
}

void Window::relMouse(const bool status) {
  assert(SDL_GetMouseFocus() == window);
  CHECK_SDL_ERROR(SDL_SetRelativeMouseMode(static_cast<SDL_bool>(status)));
}

bool Window::relMouse() const {
  return SDL_GetRelativeMouseMode() && SDL_GetMouseFocus() == window;
}

void Window::captureMouse(const bool status) {
  assert(SDL_GetMouseFocus() == window);
  CHECK_SDL_ERROR(SDL_CaptureMouse(static_cast<SDL_bool>(status)));
  mouseCaptured = status;
}

bool Window::captureMouse() const {
  return mouseCaptured;
}

void Window::raise() {
  SDL_RaiseWindow(window);
}
