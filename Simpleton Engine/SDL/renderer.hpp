//
//  renderer.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 20/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_sdl_renderer_hpp
#define engine_sdl_renderer_hpp

#include <glm/vec4.hpp>
#include <SDL2/SDL_render.h>
#include "../Utils/generic raii.hpp"

namespace SDL {
  class Renderer {
  public:
    UTILS_RAII_CLASS_FULL(Renderer, SDL_Renderer *, renderer, SDL_DestroyRenderer)
    
    void setColor(glm::tvec4<uint8_t>);
    glm::tvec4<uint8_t> getColor() const;
  
    void present();
    void clear(glm::tvec4<uint8_t> = {0, 0, 0, 0});

  private:
    SDL_Renderer *renderer;
  };
}

#include "renderer.inl"

#endif
