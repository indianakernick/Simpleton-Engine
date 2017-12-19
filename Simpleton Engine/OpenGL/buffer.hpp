//
//  buffer.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_buffer_hpp
#define engine_opengl_buffer_hpp

#include "opengl.hpp"
#include "generic raii.hpp"

namespace GL {
  namespace detail {
    inline void deleteBuffer(const GLuint &id) {
      glDeleteBuffers(1, &id);
    }
  }

  class Buffer {
  public:
    RAII_CLASS_MEMBERS(Buffer, GLuint, id, detail::deleteBuffer)
    
    void bind(GLenum = GL_ARRAY_BUFFER) const;
  
  private:
    GLuint id;
  };
  
  Buffer makeBuffer();
  Buffer makeBuffer(GLenum, size_t, GLenum = GL_STATIC_DRAW);
  Buffer makeBuffer(GLenum, const GLvoid *, size_t, GLenum = GL_STATIC_DRAW);
}

#include "buffer.inl"

#endif
