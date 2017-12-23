//
//  shader.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_shader_hpp
#define engine_opengl_shader_hpp

#include <iostream>
#include "opengl.hpp"
#include "../Utils/generic raii.hpp"

namespace GL {
  namespace detail {
    void deleteShader(GLuint);
  }

  template <GLenum TYPE_>
  class Shader {
  public:
    UTILS_RAII_CLASS(Shader, GLuint, id, detail::deleteShader)
  
    static constexpr GLenum TYPE = TYPE_;
  
    void uploadSource(const GLchar *, size_t) const;
    bool compile() const;
  
    template <GLenum TYPE>
    friend std::ostream &operator<<(std::ostream &, const Shader<TYPE> &);
    
  private:
    GLuint id;
  };
  
  using VertShader = Shader<GL_VERTEX_SHADER>;
  using FragShader = Shader<GL_FRAGMENT_SHADER>;
  
  template <GLenum TYPE>
  struct ShaderName {};
  
  template <>
  struct ShaderName<GL_VERTEX_SHADER> {
    constexpr static const char *const value = "vertex";
  };
  
  template <>
  struct ShaderName<GL_FRAGMENT_SHADER> {
    constexpr static const char *const value = "fragment";
  };
  
  template <GLenum TYPE>
  std::ostream &operator<<(std::ostream &, const Shader<TYPE> &);
  template <GLenum TYPE>
  std::istream &operator>>(std::istream &, const Shader<TYPE> &);
  
  template <GLenum TYPE>
  Shader<TYPE> makeShader();
  template <GLenum TYPE>
  Shader<TYPE> makeShader(const GLchar *, size_t);
  template <GLenum TYPE>
  Shader<TYPE> makeShader(std::istream &);
  
  VertShader makeVertShader();
  VertShader makeVertShader(const GLchar *, size_t);
  VertShader makeVertShader(std::istream &);
  
  FragShader makeFragShader();
  FragShader makeFragShader(const GLchar *, size_t);
  FragShader makeFragShader(std::istream &);
}

#include "shader.inl"

#endif
