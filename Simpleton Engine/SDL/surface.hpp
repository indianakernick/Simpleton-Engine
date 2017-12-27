//
//  surface.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 27/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_sdl_surface_hpp
#define engine_sdl_surface_hpp

#include "error.hpp"
#include "../Utils/generic raii.hpp"

extern "C" struct SDL_Surface;
extern "C" void SDL_FreeSurface(SDL_Surface *);

namespace SDL {
  class Surface {
  public:
    UTILS_RAII_CLASS(Surface, SDL_Surface *, surface, SDL_FreeSurface)
    
  private:
    SDL_Surface *surface;
  };
  
  inline Surface makeSurface(SDL_Surface *const surface) {
    return Surface(CHECK_SDL_NULL(surface));
  }
}

#endif
