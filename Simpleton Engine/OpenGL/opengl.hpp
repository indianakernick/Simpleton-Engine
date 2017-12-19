//
//  opengl.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_opengl_hpp
#define engine_opengl_opengl_hpp

#include <cstdio>
#include <cassert>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define CHECK_OPENGL_ERROR()                                                    \
for (GLenum error; (error = glGetError()) != GL_NO_ERROR;) {                    \
  std::printf("OpenGL error: %s\n", gluErrorString(error));                     \
  assert(false);                                                                \
}

#endif
