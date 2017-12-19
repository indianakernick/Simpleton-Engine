//
//  shader program.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

inline void GL::ShaderProgram::link() const {
  glLinkProgram(id);
  
  GLint status;
  glGetProgramiv(id, GL_LINK_STATUS, &status);
  if (status == 0) {
    std::cerr << "Failed to link program\n";
  }

  CHECK_OPENGL_ERROR();
}

inline void GL::ShaderProgram::printInfoLog() const {
  GLint logLength;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength) {
    const auto log = std::make_unique<char[]>(logLength);
    glGetShaderInfoLog(id, logLength, nullptr, log.get());
    std::cerr << "Shader program info log:\n" << log.get() << '\n';
  }

  CHECK_OPENGL_ERROR();
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

inline GL::ShaderProgram GL::makeShaderProgram() {
  ShaderProgram program(glCreateProgram());
  CHECK_OPENGL_ERROR();
  return program;
}
