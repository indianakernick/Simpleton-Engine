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
  class SDLApp : public App {
  public:
    SDLApp() = default;
    virtual ~SDLApp() = default;

    std::unique_ptr<Platform::WindowLibrary> winLib;
    std::unique_ptr<Platform::FontLibrary> fontLib;
    Platform::Window window;
    Platform::Renderer renderer;

  protected:
    void initWindow(const Platform::Window::Desc &, bool);
    void quitWindow();
  };
}

#endif
