//
//  window.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 2/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_sdl_window_hpp
#define engine_sdl_window_hpp

#include <string>
#include <glm/vec2.hpp>
#include "../Utils/generic raii.hpp"

extern "C" struct SDL_Window;
extern "C" void SDL_DestroyWindow(SDL_Window *);

namespace SDL {
  class Window {
  public:
    using ID = uint32_t;
  
    struct Desc {
      std::string title;
      glm::ivec2 size = {1280, 720};
      bool resizable = false;
      bool openGL = false;
    };
  
    UTILS_RAII_CLASS(Window, SDL_Window *, window, SDL_DestroyWindow)
    
    ID getID() const;
    
    void title(const std::string &);
    std::string title() const;
    
    void center();
    void pos(glm::ivec2);
    glm::ivec2 pos() const;
    
    void size(glm::ivec2);
    glm::ivec2 size() const;
    
    void relMouse(bool);
    bool relMouse() const;
    
    void captureMouse(bool);
    bool captureMouse() const;
    
    void raise();
    
  private:
    SDL_Window *window;
    bool mouseCaptured = false;
  };
}

#endif
