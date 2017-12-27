//
//  font library.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_sdl_font_library_hpp
#define engine_sdl_font_library_hpp

#include "error.hpp"
#include <SDL2/SDL_ttf.h>
#include "../Utils/generic raii.hpp"
#include "../Utils/instance limiter.hpp"

namespace SDL {
  namespace detail {
    inline void deleteFontLibrary(const bool initialized) {
      if (initialized) {
        TTF_Quit();
      }
    }
  }

  class FontLibrary : public Utils::ForceSingleton<FontLibrary> {
  public:
    UTILS_RAII_CLASS(FontLibrary, bool, initialized, detail::deleteFontLibrary)
    
  private:
    bool initialized;
  };
  
  FontLibrary makeFontLibrary();
  
  using Font = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>;
  
  Font openFont(const std::string &, int);
}

#endif
