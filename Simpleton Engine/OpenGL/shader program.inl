//
//  shader program.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "static char buffer.hpp"

inline void GL::detail::deleteProgram(const GLuint id) {
  glDeleteProgram(id);
  
  CHECK_OPENGL_ERROR();
}

inline bool GL::ShaderProgram::link() const {
  glLinkProgram(id);
  GLint status;
  glGetProgramiv(id, GL_LINK_STATUS, &status);
  CHECK_OPENGL_ERROR();
  return status == GL_TRUE;
}

inline bool GL::ShaderProgram::validate() const {
  glValidateProgram(id);
  GLint status;
  glGetProgramiv(id, GL_VALIDATE_STATUS, &status);
  CHECK_OPENGL_ERROR();
  return status == GL_TRUE;
}

inline void GL::ShaderProgram::use() const {
  glUseProgram(id);
  
  CHECK_OPENGL_ERROR();
}

inline void GL::ShaderProgram::attach(const Shader &shader) const {
  attach(shader.get());
}

inline void GL::ShaderProgram::attach(const GLuint shaderID) const {
  glAttachShader(id, shaderID);
  
  CHECK_OPENGL_ERROR();
}

inline void GL::ShaderProgram::detach(const Shader &shader) const {
  detach(shader.get());
}

inline void GL::ShaderProgram::detach(const GLuint shaderID) const {
  glDetachShader(id, shaderID);
  
  CHECK_OPENGL_ERROR();
}

inline std::ostream &GL::operator<<(std::ostream &stream, const GL::ShaderProgram &program) {
  GLint logLength;
  glGetProgramiv(program.id, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength) {
    GLchar *const buf = detail::getCharBuf(logLength);
    glGetProgramInfoLog(program.id, logLength, nullptr, buf);
    //avoiding a call to strlen
    stream << std::string_view(buf, logLength);
  }

  CHECK_OPENGL_ERROR();
  
  return stream;
}

inline GL::ShaderProgram GL::makeShaderProgram() {
  ShaderProgram program(glCreateProgram());
  CHECK_OPENGL_ERROR();
  return program;
}
