//
//  buffer.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

template <GLenum TARGET>
GL::Buffer<TARGET> GL::makeBuffer() {
  GLuint id;
  glGenBuffers(1, &id);
  Buffer<TARGET> buffer(id);
  CHECK_OPENGL_ERROR();
  return buffer;
}

template <GLenum TARGET>
GL::Buffer<TARGET> GL::makeBuffer(const size_t size, const GLenum usage) {
  return makeBuffer<TARGET>(nullptr, size, usage);
}

template <GLenum TARGET>
GL::Buffer<TARGET> GL::makeBuffer(
  const GLvoid *data,
  const size_t size,
  const GLenum usage
) {
  Buffer<TARGET> buffer = makeBuffer<TARGET>();
  buffer.bind();
  glBufferData(TARGET, static_cast<GLsizeiptr>(size), data, usage);
  CHECK_OPENGL_ERROR();
  return buffer;
}
