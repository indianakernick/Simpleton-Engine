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
  namespace detail {
    inline void deleteProgram(const GLuint id) {
      glDeleteProgram(id);
      
      CHECK_OPENGL_ERROR();
    }
  }

  class ShaderProgram {
  public:
    RAII_CLASS_MEMBERS(ShaderProgram, GLuint, id, detail::deleteProgram)
    
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
  std::enable_if_t<
    sizeof...(Shaders) != 0
      && (std::is_same_v<Shaders, Shader> && ...),
    ShaderProgram
  >
  makeShaderProgram(const Shaders &... shaders) {
    ShaderProgram program = makeShaderProgram();
    [[maybe_unused]]
    const int dummy0[] = {(program.attach(shaders), 0)...};
    program.link();
    [[maybe_unused]]
    const int dummy1[] = {(program.detach(shaders), 0)...};
    program.printInfoLog();
    return program;
  }

  template <typename ...Shaders>
  std::enable_if_t<
    sizeof...(Shaders) != 0
      && (std::is_base_of_v<std::istream, Shaders> && ...),
    ShaderProgram
  >
  makeShaderProgram(const Shaders &... streams) {
    return makeShaderProgram(makeShader(streams)...);
  }
}

#include "shader program.inl"

#endif
