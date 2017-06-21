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
#include <SDL2/SDL_render.h>
#include "../Utils/implicit unique ptr.hpp"

namespace Platform {
  class Renderer : public Utils::ImplicitUniquePtr<SDL_Renderer, &SDL_DestroyRenderer> {
  public:
    //not explicit
    Renderer(SDL_Renderer *);
  
    void present();
    void clear();
  };
}

#endif
