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
#include "generic raii.hpp"

namespace GL {
  namespace detail {
    inline void deleteShader(const GLuint id) {
      glDeleteShader(id);
      
      CHECK_OPENGL_ERROR();
    }
  }

  class Shader {
  public:
    RAII_CLASS_MEMBERS(Shader, GLuint, id, detail::deleteShader)
  
    void uploadSource(const GLchar *, size_t) const;
    bool compile() const;
  
    friend std::ostream &operator<<(std::ostream &, const Shader &);
    
  private:
    GLuint id;
  };
  
  std::ostream &operator<<(std::ostream &, const Shader &);
  std::istream &operator>>(std::istream &, const Shader &);
  
  Shader makeShader(GLenum);
  Shader makeShader(GLenum, const GLchar *, size_t);
  Shader makeShader(GLenum, std::istream &);
}

#endif
