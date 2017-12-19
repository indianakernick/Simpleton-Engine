//
//  shader.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

inline void GL::Shader::uploadSource(const GLchar *source, const size_t size) const {
  const GLint length = static_cast<GLint>(size);
  glShaderSource(id, 1, &source, &length);
  CHECK_OPENGL_ERROR();
}

inline void GL::Shader::compile() const {
  glCompileShader(id);

  GLint status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status == 0) {
    std::cerr << "Failed to compile shader\n";
  }

  CHECK_OPENGL_ERROR();
}

inline void GL::Shader::printInfoLog() const {
  GLint logLength;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength) {
    const auto log = std::make_unique<char[]>(logLength);
    glGetShaderInfoLog(id, logLength, nullptr, log.get());
    std::cerr << "Shader info log:\n" << log.get() << '\n';
  }

  CHECK_OPENGL_ERROR();
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
  shader.compile();
  shader.printInfoLog();
  return shader;
}
