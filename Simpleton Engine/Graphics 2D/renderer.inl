//
//  renderer.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 11/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include "shaders.hpp"

#include "../OpenGL/uniforms.hpp"
#include "../OpenGL/attrib pointer.hpp"

inline void G2D::Renderer::initCore() {
  if (program == nullptr) {
    initState();
    program = GL::makeShaderProgram(
      GL::makeVertShader(CORE_SHADER_VERSION, VERT_SHADER),
      GL::makeFragShader(CORE_SHADER_VERSION, FRAG_SHADER)
    );
    initUniforms();
  }
  
  initVertexArray();
}

inline void G2D::Renderer::initES() {
  if (program == nullptr) {
    initState();
    program = GL::makeShaderProgram(
      GL::makeVertShader(ES_SHADER_VERSION, VERT_SHADER),
      GL::makeFragShader(ES_SHADER_VERSION, FRAG_SHADER)
    );
    initUniforms();
  }
  
  initVertexArray();
}

inline void G2D::Renderer::quit() {
  elemBuf.reset();
  arrayBuf.reset();
  vertArray.reset();
  textures.clear();
}

inline G2D::TextureID G2D::Renderer::addTexture(GL::Texture2D &&texture) {
  const TextureID id = textures.size();
  textures.emplace_back(std::move(texture));
  return id;
}

inline G2D::QuadIter G2D::Renderer::getQuadBuf() {
  return quads.begin();
}

inline size_t G2D::Renderer::getQuadBufSize() const {
  return quads.size();
}

inline void G2D::Renderer::setQuadBufSize(const size_t numQuads) {
  fillIndicies(numQuads);
  quads.resize(numQuads);
  
  arrayBuf = GL::makeArrayBuffer(numQuads * QUAD_ATTR_SIZE, GL_DYNAMIC_DRAW);
  elemBuf = GL::makeElementBuffer(indicies.data(), numQuads * QUAD_ELEM_SIZE);
  
  writeElements();
}

inline void G2D::Renderer::initState() {
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

inline void G2D::Renderer::initUniforms() {
  viewProjLoc = program.getUniformLoc("viewProj");
  texLoc = program.getUniformLoc("tex");
  colorLoc = program.getUniformLoc("color");
  
  program.use();
  GL::setUniform(texLoc, 0);
  GL::unuseProgram();
}

inline void G2D::Renderer::initVertexArray() {
  vertArray = GL::makeVertexArray();
  vertArray.bind();
  
  arrayBuf = GL::makeArrayBuffer(size_t(0), GL_DYNAMIC_DRAW);
  elemBuf = GL::makeElementBuffer(size_t(0), GL_STATIC_DRAW);
  
  GL::attribs<Attribs>();
  
  GL::unbindVertexArray();
}
