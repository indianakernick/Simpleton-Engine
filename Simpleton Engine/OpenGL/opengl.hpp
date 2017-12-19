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
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdeprecated-declarations"
    return reinterpret_cast<const char *>(gluErrorString(error));
    #pragma clang diagnostic pop
  }
}

#define CHECK_OPENGL_ERROR()                                                    \
  for (GLenum error; (error = glGetError()) != GL_NO_ERROR;) {                  \
    std::cerr << "OpenGL error: " << GL::detail::glErrorString(error) << '\n';  \
    assert(false);                                                              \
  }

#endif // DISABLE_OPENGL_ERROR_CHECKING

#endif
