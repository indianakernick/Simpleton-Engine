//
//  renderer.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 20/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "renderer.hpp"

using namespace Platform;

Renderer::Renderer(SDL_Renderer *renderer)
  : ImplicitUniquePtr(renderer) {
  assert(renderer);
}

void Renderer::present() {
  SDL_RenderPresent(ptr);
}

void Renderer::clear() {
  SDL_SetRenderDrawColor(ptr, 0, 0, 0, 0);
  SDL_RenderClear(ptr);
}
