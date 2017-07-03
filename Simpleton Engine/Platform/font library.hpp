//
//  font library.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_font_library_hpp
#define engine_platform_font_library_hpp

#include <SDL2/SDL_ttf.h>
#include "../Utils/instance limiter.hpp"

namespace Platform {
  class FontLibInitError : public std::runtime_error {
  public:
    explicit FontLibInitError(const char *);
  };
  
  class FontOpenError : public std::runtime_error {
  public:
    explicit FontOpenError(const char *);
  };
  
  using Font = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>;
  
  class FontLibrary : public Utils::ForceSingleton<FontLibrary> {
  public:
    FontLibrary();
    ~FontLibrary();
    
    Font openFont(const std::string &, int);
  };
}

#endif
