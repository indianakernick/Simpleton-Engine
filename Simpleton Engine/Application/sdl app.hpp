//
//  sdl app.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 9/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_application_sdl_app_hpp
#define engine_application_sdl_app_hpp

#include "app.hpp"
#include "../Platform/window library.hpp"
#include "../Platform/font library.hpp"

namespace Game {
  template <typename Duration>
  class SDLApp : public App<Duration> {
  public:
    SDLApp() = default;
    virtual ~SDLApp() = default;

    Platform::WindowLibrary winLib;
    Platform::FontLibrary fontLib;
    Platform::Window window;
    Platform::Renderer renderer;

  protected:
    void initWindow(const Platform::Window::Desc &winDesc, const bool vsync) {
      winLib = Platform::makeWindowLibrary();
      fontLib = Platform::makeFontLibrary();
      window = Platform::makeWindow(winDesc);
      renderer = Platform::makeRenderer(window, vsync);
    }
    void quitWindow() {
      renderer.reset();
      window.reset();
      fontLib.reset();
      winLib.reset();
    }
  };
}

#endif
