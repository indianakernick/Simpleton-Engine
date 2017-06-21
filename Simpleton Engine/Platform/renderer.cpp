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

void Renderer::setColor(const glm::tvec4<uint8_t> color) {
  SDL_SetRenderDrawColor(ptr, color.r, color.g, color.b, color.a);
}

glm::tvec4<uint8_t> Renderer::getColor() const {
  glm::tvec4<uint8_t> color;
  SDL_GetRenderDrawColor(ptr, &color.r, &color.g, &color.b, &color.a);
  return color;
}

void Renderer::present() {
  SDL_RenderPresent(ptr);
}

void Renderer::clear() {
  SDL_SetRenderDrawColor(ptr, 0, 0, 0, 0);
  SDL_RenderClear(ptr);
}
