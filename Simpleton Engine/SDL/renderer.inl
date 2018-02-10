//
//  renderer.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 20/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include <cassert>
#include "error.hpp"

inline void SDL::Renderer::setColor(const glm::tvec4<uint8_t> color) {
  CHECK_SDL_ERROR(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a));
}

inline glm::tvec4<uint8_t> SDL::Renderer::getColor() const {
  glm::tvec4<uint8_t> color;
  CHECK_SDL_ERROR(SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a));
  return color;
}

inline void SDL::Renderer::present() {
  SDL_RenderPresent(renderer);
}

inline void SDL::Renderer::clear(const glm::tvec4<uint8_t> color) {
  CHECK_SDL_ERROR(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a));
  CHECK_SDL_ERROR(SDL_RenderClear(renderer));
}
