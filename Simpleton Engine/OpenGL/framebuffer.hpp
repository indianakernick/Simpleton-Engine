//
//  framebuffer.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 23/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_framebuffer_hpp
#define engine_opengl_framebuffer_hpp

#include "texture.hpp"

namespace GL {
  namespace detail {
    void deleteFramebuffer(const GLuint &);
  }

  class Framebuffer {
  public:
    UTILS_RAII_CLASS(Framebuffer, GLuint, id, detail::deleteFramebuffer)
    
    void bind() const;
    bool checkStatus() const;
    void attachColor(const Texture2D &, size_t) const;
  
  private:
    GLuint id;
  };
  
  void makeFramebuffer();
}

#endif
