//
//  renderer.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 20/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "renderer.hpp"

#include <cassert>
#include "error.hpp"
#include <SDL2/SDL_render.h>

using namespace SDL;

void Renderer::setColor(const glm::tvec4<uint8_t> color) {
  CHECK_SDL_ERROR(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a));
}

glm::tvec4<uint8_t> Renderer::getColor() const {
  glm::tvec4<uint8_t> color;
  CHECK_SDL_ERROR(SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a));
  return color;
}

void Renderer::present() {
  SDL_RenderPresent(renderer);
}

void Renderer::clear(const glm::tvec4<uint8_t> color) {
  CHECK_SDL_ERROR(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a));
  CHECK_SDL_ERROR(SDL_RenderClear(renderer));
}
