//
//  sdl object.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_sdl_object_hpp
#define engine_platform_sdl_object_hpp

#include <memory>

#define SDL_OBJECT_FREE(TYPE) std::unique_ptr<SDL_##TYPE, decltype(&SDL_Free##TYPE)>
#define SDL_OBJECT_DESTROY(TYPE) std::unique_ptr<SDL_##TYPE, decltype(&SDL_Destroy##TYPE)>

#define MAKE_SDL_OBJECT_FREE(TYPE, NAME) SDL_OBJECT_FREE(TYPE) NAME(nullptr, &SDL_Free##TYPE)
#define MAKE_SDL_OBJECT_DESTROY(TYPE, NAME) SDL_OBJECT_DESTROY(TYPE) NAME(nullptr, &SDL_Destroy##TYPE)

#endif
