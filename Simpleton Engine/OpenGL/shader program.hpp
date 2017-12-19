//
//  shader program.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_shader_program_hpp
#define engine_opengl_shader_program_hpp

#include "shader.hpp"

namespace GL {
  class ShaderProgram {
  public:
    RAII_CLASS_MEMBERS(ShaderProgram, GLuint, id, glDeleteProgram)
    
    void link() const;
    void printInfoLog() const;
    void use() const;
    void attach(const Shader &) const;
    void attach(GLuint) const;
    void detach(const Shader &) const;
    void detach(GLuint) const;
    
  private:
    GLuint id;
  };
  
  ShaderProgram makeShaderProgram();
  template <typename ...Shaders>
  ShaderProgram makeShaderProgram(const Shaders &...);
}

#include "shader program.inl"

#endif
