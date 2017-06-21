//
//  renderer.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 20/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_renderer_hpp
#define engine_platform_renderer_hpp

#include <memory>
#include <cassert>
#include <glm/vec4.hpp>
#include <SDL2/SDL_render.h>
#include "../Utils/implicit unique ptr.hpp"

namespace Platform {
  class Renderer : public Utils::ImplicitUniquePtr<SDL_Renderer, &SDL_DestroyRenderer> {
  public:
    //not explicit
    Renderer(SDL_Renderer *);
  
    void setColor(glm::tvec4<uint8_t>);
    glm::tvec4<uint8_t> getColor() const;
  
    void present();
    void clear();
  };
}

#endif
