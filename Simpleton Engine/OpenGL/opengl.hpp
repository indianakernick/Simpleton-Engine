//
//  opengl.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_opengl_hpp
#define engine_opengl_opengl_hpp

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef DISABLE_OPENGL_ERROR_CHECKING

#define CHECK_OPENGL_ERROR()

#else

#include <cassert>
#include <iostream>

namespace GL::detail {
  inline const char *glErrorString(const GLenum error) {
    switch (error) {
      case GL_INVALID_ENUM:
        return "Invalid enum";
      case GL_INVALID_VALUE:
        return "Invalid value";
      case GL_INVALID_OPERATION:
        return "Invalid operation";
      case GL_STACK_OVERFLOW:
        return "Stack overflow";
      case GL_STACK_UNDERFLOW:
        return "Stack underflow";
      case GL_OUT_OF_MEMORY:
        return "Out of memory";
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "Invalid framebuffer operation";
      case GL_CONTEXT_LOST:
        return "Context lost";
      case GL_TABLE_TOO_LARGE:
        return "Table too large";
      default:
        assert(false);
    }
  }
}

#define CHECK_OPENGL_ERROR()                                                    \
  do {                                                                          \
    int errorCount = 0;                                                         \
    for (GLenum error; (error = glGetError()) != GL_NO_ERROR;) {                \
      std::cerr << "OpenGL error: " << GL::detail::glErrorString(error) << '\n';\
    }                                                                           \
    assert(errorCount == 0);                                                    \
  } while (false)

#endif // DISABLE_OPENGL_ERROR_CHECKING

#endif
