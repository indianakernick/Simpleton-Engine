//
//  context.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_context_hpp
#define engine_opengl_context_hpp

#include "opengl.hpp"

namespace GL {
  class Context {
  public:
    explicit Context(SDL_GLContext);
    Context(Context &&);
    Context &operator=(Context &&);
    ~Context();
  
  private:
    SDL_GLContext context;
  };
  
  struct ContextParams {
    int stencilBits = 8;
    int depthBits = 16;
    int colorBits = 32;
    int majorVersion = 3;
    int minorVersion = 3;
    bool vsync = true;
  };
  
  Context makeContext(SDL_Window *, const ContextParams &);
}

#include "context.inl"

#endif
