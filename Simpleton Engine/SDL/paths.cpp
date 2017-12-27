//
//  paths.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 5/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "paths.hpp"

#include <SDL2/SDL.h>

std::string SDL::getSaveDir(
  const std::string_view companyName,
  const std::string_view appName
) {
  char *cstr = SDL_GetPrefPath(companyName.data(), appName.data());
  std::string str(cstr);
  SDL_free(cstr);
  return str;
}

std::string SDL::getResDir() {
  char *cstr = SDL_GetBasePath();
  std::string str(cstr);
  SDL_free(cstr);
  return str;
}
