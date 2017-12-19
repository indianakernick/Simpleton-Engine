//
//  buffer.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

inline void GL::Buffer::bind(const GLenum target) const {
  glBindBuffer(target, id);
  
  CHECK_OPENGL_ERROR();
}

inline GL::Buffer GL::makeBuffer() {
  GLuint id;
  glGenBuffers(1, &id);
  Buffer buffer(id);
  CHECK_OPENGL_ERROR();
  return buffer;
}

inline GL::Buffer GL::makeBuffer(
  const GLenum target,
  const size_t size,
  const GLenum usage
) {
  return makeBuffer(target, nullptr, size, usage);
}

inline GL::Buffer GL::makeBuffer(
  const GLenum target,
  const GLvoid *data,
  const size_t size,
  const GLenum usage
) {
  Buffer buffer = makeBuffer();
  buffer.bind(target);
  glBufferData(target, static_cast<GLsizeiptr>(size), data, usage);
  CHECK_OPENGL_ERROR();
  return buffer;
}
