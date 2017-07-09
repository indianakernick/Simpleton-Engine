//
//  window library.cpp
//  Game Engine
//
//  Created by Indi Kernick on 3/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "window library.hpp"

using namespace Platform;

WindowLibInitError::WindowLibInitError(const char *what)
  : std::runtime_error(what) {}

WindowOpenError::WindowOpenError(const char *what)
  : std::runtime_error(what) {}

RendererCreateError::RendererCreateError(const char *what)
  : std::runtime_error(what) {}

Platform::WindowLibrary::WindowLibrary() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    throw WindowLibInitError(SDL_GetError());
  }
}

Platform::WindowLibrary::~WindowLibrary() {
  SDL_Quit();
}

Window Platform::makeWindow(const Window::Desc &desc) {
  SDL_Window *window = SDL_CreateWindow(
    desc.title.c_str(),
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    desc.size.x, desc.size.y,
    SDL_WINDOW_SHOWN |
    Utils::boolEnable(desc.resizable, SDL_WINDOW_RESIZABLE) |
    Utils::boolEnable(desc.openGL, SDL_WINDOW_OPENGL)
  );
  
  if (window == nullptr) {
    throw WindowOpenError(SDL_GetError());
  }
  
  return Window(window);
}

Renderer Platform::makeRenderer(Window &window, const bool vsync) {
  SDL_Renderer *renderer = SDL_CreateRenderer(
    window.get(),
    -1,
    SDL_RENDERER_ACCELERATED |
    Utils::boolEnable(vsync, SDL_RENDERER_PRESENTVSYNC)
  );
  
  if (renderer == nullptr) {
    throw RendererCreateError(SDL_GetError());
  }
  
  return Renderer(renderer);
}
