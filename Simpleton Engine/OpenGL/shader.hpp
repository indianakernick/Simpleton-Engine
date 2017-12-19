//
//  shader.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_shader_hpp
#define engine_opengl_shader_hpp

#include "opengl.hpp"
#include "generic raii.hpp"

namespace GL {
  class Shader {
  public:
    RAII_CLASS_MEMBERS(Shader, GLuint, id, glDeleteShader)
  
    void uploadSource(const GLchar *, size_t) const;
    void uploadSource(std::istream &) const;
    void compile() const;
    void printInfoLog() const;
  
  private:
    GLuint id;
  };
  
  Shader makeShader(GLenum);
  Shader makeShader(GLenum, const GLchar *, size_t);
  Shader makeShader(GLenum, std::istream &);
}

#endif
