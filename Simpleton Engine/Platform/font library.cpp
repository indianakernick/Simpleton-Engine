//
//  font library.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "font library.hpp"

#include <string>

Platform::FontLibrary Platform::makeFontLibrary() {
  //SDL_TTF uses SDL's error system
  CHECK_SDL_ERROR(TTF_Init());
  return Platform::FontLibrary(true);
}

Platform::Font Platform::openFont(const std::string &path, const int size) {
  return {CHECK_SDL_NULL(TTF_OpenFont(path.c_str(), size)), &TTF_CloseFont};
}
