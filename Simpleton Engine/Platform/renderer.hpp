//
//  renderer.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 20/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_renderer_hpp
#define engine_platform_renderer_hpp

#include <glm/vec4.hpp>
#include "../Utils/generic raii.hpp"

extern "C" struct SDL_Renderer;
extern "C" void SDL_DestroyRenderer(SDL_Renderer *);

namespace Platform {
  class Renderer {
  public:
    UTILS_RAII_CLASS(Renderer, SDL_Renderer *, renderer, SDL_DestroyRenderer)
    
    void setColor(glm::tvec4<uint8_t>);
    glm::tvec4<uint8_t> getColor() const;
  
    void present();
    void clear(glm::tvec4<uint8_t> = {0, 0, 0, 0});

  private:
    SDL_Renderer *renderer;
  };
}

#endif
