//
//  window library.cpp
//  Game Engine
//
//  Created by Indi Kernick on 3/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "window library.hpp"

using namespace Platform;

LibInitError::LibInitError(const char *what)
  : std::runtime_error(what) {}

void Platform::initLib() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    throw LibInitError(SDL_GetError());
  }
}

void Platform::quitLib() {
  SDL_Quit();
}

Renderer::Ptr Platform::makeRenderer(const Window::Ptr window, const bool vsync) {
  return std::make_shared<Renderer>(
    SDL_CreateRenderer(
      window->get(),
      -1,
      SDL_RENDERER_ACCELERATED |
      Utils::boolEnable(vsync, SDL_RENDERER_PRESENTVSYNC)
    )
  );
}

