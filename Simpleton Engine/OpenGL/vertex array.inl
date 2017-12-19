//
//  vertex array.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "vertex array.hpp"

void GL::VertexArray::bind() const {
  glBindVertexArray(id);
  
  CHECK_OPENGL_ERROR();
}

GL::VertexArray GL::makeVertexArray() {
  GLuint id;
  glGenVertexArrays(1, &id);
  VertexArray vertexArray(id);
  CHECK_OPENGL_ERROR();
  return vertexArray;
}
