//
//  window.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 2/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_sdl_opengl_window_hpp
#define engine_platform_sdl_opengl_window_hpp

#include <string>
#include <glm/vec2.hpp>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_mouse.h>
#include "../Utils/bool enable.hpp"

namespace Platform {
  class Window {
  public:
    using ID = Uint32;
  
    struct Desc {
      std::string title;
      glm::ivec2 size = {1280, 720};
      bool resizable = false;
      bool openGL = false;
    };
  
    Window();
    explicit Window(SDL_Window *);
    Window(Window &&) = default;
    ~Window() = default;
    
    Window &operator=(Window &&) = default;
    
    ID getID() const;
    
    void title(const std::string &);
    std::string title() const;
    
    void pos(glm::ivec2);
    glm::ivec2 pos() const;
    
    void center();
    void center(bool, bool);
    
    void size(glm::ivec2);
    glm::ivec2 size() const;
    
    void opacity(float);
    float opacity() const;
    
    void relMouse(bool);
    bool relMouse() const;
    
    void captureMouse(bool);
    bool captureMouse() const;
    
    void raise();
    
    SDL_Window *get() const;
    void reset(SDL_Window * = nullptr);
    
  private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    bool mouseCaptured = false;
  };
}

#endif
