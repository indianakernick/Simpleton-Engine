//
//  window library.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 3/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_window_library_hpp
#define engine_platform_window_library_hpp

#include <SDL2/SDL.h>
#include "window.hpp"
#include "renderer.hpp"
#include "sdl error.hpp"
#include "../Utils/generic raii.hpp"
#include "../Utils/instance limiter.hpp"

namespace Platform {
  namespace detail {
    inline void deleteWindowLibrary(const bool initialized) {
      if (initialized) {
        SDL_Quit();
      }
    }
  }

  class WindowLibrary final : public Utils::ForceSingleton<WindowLibrary> {
  public:
    UTILS_RAII_CLASS(WindowLibrary, bool, initialized, detail::deleteWindowLibrary)
    
  private:
    bool initialized;
  };
  
  WindowLibrary makeWindowLibrary(Uint32 = SDL_INIT_EVERYTHING);
  
  Window makeWindow(const Window::Desc &);
  Renderer makeRenderer(Window &, bool);
}

#endif
