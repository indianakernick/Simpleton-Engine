//
//  sdl app.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 9/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "sdl app.hpp"

void Game::SDLApp::initWindow(const Platform::Window::Desc &winDesc, const bool vsync) {
  winLib = std::make_unique<Platform::WindowLibrary>();
  fontLib = std::make_unique<Platform::FontLibrary>();
  window = Platform::makeWindow(winDesc);
  renderer = Platform::makeRenderer(window, vsync);
}

void Game::SDLApp::quitWindow() {
  renderer.reset();
  window.reset();
  fontLib.reset();
  winLib.reset();
}
