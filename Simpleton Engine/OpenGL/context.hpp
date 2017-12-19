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
    Context();
    explicit Context(SDL_GLContext);
    Context(Context &&);
    Context &operator=(Context &&);
    Context &operator=(std::nullptr_t);
    ~Context();
  
    void makeCurrent(SDL_Window *) const;
  
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
  void clearFrame();
}

#include "context.inl"

#endif
