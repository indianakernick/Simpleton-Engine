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
  : renderer(renderer, &SDL_DestroyRenderer) {
  assert(renderer);
}

void Renderer::setColor(const glm::tvec4<uint8_t> color) {
  SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);
}

glm::tvec4<uint8_t> Renderer::getColor() const {
  glm::tvec4<uint8_t> color;
  SDL_GetRenderDrawColor(renderer.get(), &color.r, &color.g, &color.b, &color.a);
  return color;
}

void Renderer::present() {
  SDL_RenderPresent(renderer.get());
}

void Renderer::clear() {
  SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);
  SDL_RenderClear(renderer.get());
}

SDL_Renderer *Renderer::get() const {
  return renderer.get();
}

void Renderer::reset(SDL_Renderer *newRenderer) {
  renderer.reset(newRenderer);
}
