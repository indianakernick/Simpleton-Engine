//
//  shader.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "static char buffer.hpp"

inline void GL::detail::deleteShader(const GLuint id) {
  glDeleteShader(id);
  
  CHECK_OPENGL_ERROR();
}

inline void GL::Shader::uploadSource(const GLchar *source, const size_t size) const {
  const GLint length = static_cast<GLint>(size);
  glShaderSource(id, 1, &source, &length);
  
  CHECK_OPENGL_ERROR();
}

inline bool GL::Shader::compile() const {
  glCompileShader(id);
  GLint status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  CHECK_OPENGL_ERROR();
  return status == GL_TRUE;
}

inline std::ostream &GL::operator<<(std::ostream &stream, const Shader &shader) {
  GLint logLength;
  glGetShaderiv(shader.id, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength) {
    GLchar *const buf = detail::getCharBuf(logLength);
    glGetShaderInfoLog(shader.id, logLength, nullptr, buf);
    //avoiding a call to strlen
    stream << std::string_view(buf, logLength);
  }
  return stream;
}

inline std::istream &GL::operator>>(std::istream &stream, const Shader &shader) {
  stream.seekg(0, std::ios::seekdir::end);
  const size_t size = stream.tellg();
  GLchar *const source = detail::getCharBuf(static_cast<GLint>(size));
  stream.seekg(0, std::ios::seekdir::beg);
  stream.read(source, size);
  shader.uploadSource(source, size);
  return stream;
}

inline GL::Shader GL::makeShader(const GLenum type) {
  Shader shader(glCreateShader(type));
  CHECK_OPENGL_ERROR();
  return shader;
}

inline GL::Shader GL::makeShader(
  const GLenum type,
  const GLchar *source,
  const size_t size
) {
  Shader shader = makeShader(type);
  shader.uploadSource(source, size);
  if (!shader.compile()) {
    std::cerr << "Failed to compile shader\n";
  }
  std::cerr << "Shader info log:\n" << shader << '\n';
  return shader;
}

inline GL::Shader GL::makeShader(const GLenum type, std::istream &stream) {
  Shader shader = makeShader(type);
  stream >> shader;
  if (!shader.compile()) {
    std::cerr << "Failed to compile shader\n";
  }
  std::cerr << "Shader info log:\n" << shader << '\n';
  return shader;
}
