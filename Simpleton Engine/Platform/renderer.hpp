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

namespace Platform {
  class Renderer {
  public:
    using Ptr = std::shared_ptr<Renderer>;
    
    explicit Renderer(SDL_Renderer *);
  
    void present();
    void clear();
  
    SDL_Renderer *get() const;
    
  private:
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer *)> renderer;
  };
}

#endif
