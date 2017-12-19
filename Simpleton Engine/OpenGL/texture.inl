//
//  texture.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

inline void GL::Texture::bind(const GLenum target) const {
  glBindTexture(id, target);
}

inline void GL::TexParams2D::setWrap(const GLint wrap) {
  wrapS = wrap;
  wrapT = wrap;
}

inline void GL::TexParams2D::setFilter(const GLint filter) {
  minFilter = filter;
  magFilter = filter;
}

inline GL::Texture GL::makeTexture() {
  GLuint id;
  glGenTextures(1, &id);
  CHECK_OPENGL_ERROR();
  return Texture(id);
}

inline GL::Texture GL::makeTexture2D(const Image2D &image, const TexParams2D &params) {
  Texture texture = makeTexture();
  texture.bind(GL_TEXTURE_2D);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
  
  glPixelStorei(GL_UNPACK_ROW_LENGTH, image.pitch);
  
  glTexImage2D(
    GL_TEXTURE_2D,                         // target
    0,                                     // LOD
    image.alpha ? GL_SRGB_ALPHA : GL_SRGB, // internal format
    image.width,                           // width
    image.height,                          // height
    0,                                     // border
    image.alpha ? GL_RGBA : GL_RGB,        // format
    GL_UNSIGNED_BYTE,                      // type
    image.data                             // pixels
  );
  
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  
  CHECK_OPENGL_ERROR();
  
  return texture;
}
