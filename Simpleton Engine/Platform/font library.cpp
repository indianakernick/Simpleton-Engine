//
//  font library.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "font library.hpp"

Platform::FontLibInitError::FontLibInitError(const char *what)
  : std::runtime_error(what) {}

Platform::FontOpenError::FontOpenError(const char *what)
  : std::runtime_error(what) {}

Platform::FontLibrary::FontLibrary() {
  if (TTF_Init() != 0) {
    throw FontLibInitError(TTF_GetError());
  }
}

Platform::FontLibrary::~FontLibrary() {
  TTF_Quit();
}

Platform::Font Platform::openFont(const std::string &path, const int size) {
  TTF_Font *font = TTF_OpenFont(path.c_str(), size);
  if (font == nullptr) {
    throw FontOpenError(TTF_GetError());
  }
  return {font, &TTF_CloseFont};
}
