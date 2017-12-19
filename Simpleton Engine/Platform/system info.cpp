//
//  system info.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 5/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "system info.hpp"

#include <SDL2/SDL.h>

std::string Platform::getSaveDir(
  const std::string &companyName,
  const std::string &appName
) {
  char *cstr = SDL_GetPrefPath(companyName.c_str(), appName.c_str());
  std::string str(cstr);
  SDL_free(cstr);
  return str;
}

std::string Platform::getResDir() {
  char *cstr = SDL_GetBasePath();
  std::string str(cstr);
  SDL_free(cstr);
  return str;
}
