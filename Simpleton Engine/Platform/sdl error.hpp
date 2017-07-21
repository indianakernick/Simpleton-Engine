//
//  sdl error.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 21/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_sdl_error_hpp
#define engine_platform_sdl_error_hpp

namespace Platform {
  class SDLError final : public std::runtime_error {
  public:
    inline explicit SDLError(const char *what)
      : std::runtime_error(what) {}
  };
}

#define CHECK_SDL_ERROR(EXP) ((EXP) != 0 ? throw Platform::SDLError(SDL_GetError()) : void())

#endif
