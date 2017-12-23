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
#include "../Utils/generic raii.hpp"

namespace GL {
  namespace detail {
    void deleteBuffer(const GLuint &);
  }

  template <GLenum TARGET_>
  class Buffer {
  public:
    UTILS_RAII_CLASS(Buffer, GLuint, id, detail::deleteBuffer)
    
    static constexpr GLenum TARGET = TARGET_;
    
    void bind() const {
      glBindBuffer(TARGET, id);
      
      CHECK_OPENGL_ERROR();
    }
  
  private:
    GLuint id;
  };
  
  using ArrayBuffer = Buffer<GL_ARRAY_BUFFER>;
  using ElementBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER>;
  
  template <GLenum TARGET>
  void unbindBuffer();
  
  void unbindArrayBuffer();
  void unbindElementBuffer();
  
  template <GLenum TARGET>
  Buffer<TARGET> makeBuffer();
  template <GLenum TARGET>
  Buffer<TARGET> makeBuffer(size_t, GLenum = GL_STATIC_DRAW);
  template <GLenum TARGET>
  Buffer<TARGET> makeBuffer(const GLvoid *, size_t, GLenum = GL_STATIC_DRAW);
  
  ArrayBuffer makeArrayBuffer();
  ArrayBuffer makeArrayBuffer(size_t, GLenum = GL_STATIC_DRAW);
  ArrayBuffer makeArrayBuffer(const GLvoid *, size_t, GLenum = GL_STATIC_DRAW);
  
  ElementBuffer makeElementBuffer();
  ElementBuffer makeElementBuffer(size_t, GLenum = GL_STATIC_DRAW);
  ElementBuffer makeElementBuffer(const GLvoid *, size_t, GLenum = GL_STATIC_DRAW);
}

#include "buffer.inl"

#endif
