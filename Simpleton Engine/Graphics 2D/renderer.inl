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
  initState();
  program = GL::makeShaderProgram(
    GL::makeVertShader(CORE_SHADER_VERSION, VERT_SHADER),
    GL::makeFragShader(CORE_SHADER_VERSION, FRAG_SHADER)
  );
  initUniforms();
  initVertexArray();
}

inline void G2D::Renderer::initES() {
  initState();
  program = GL::makeShaderProgram(
    GL::makeVertShader(ES_SHADER_VERSION, VERT_SHADER),
    GL::makeFragShader(ES_SHADER_VERSION, FRAG_SHADER)
  );
  initUniforms();
  initVertexArray();
}

inline void G2D::Renderer::quit() {
  elemBuf.reset();
  arrayBuf.reset();
  vertArray.reset();
  program.reset();
  textures.clear();
}

inline G2D::TextureID G2D::Renderer::addTexture(GL::Texture2D &&texture) {
  const TextureID id = textures.size();
  textures.emplace_back(std::move(texture));
  return id;
}

inline void G2D::Renderer::writeQuads(const Quads quads) {
  if (quads.size + quads.offset > numQuads) {
    setQuadBufSize(quads.size + quads.offset);
  }
  
  arrayBuf.bind();
  glBufferSubData(
    GL_ARRAY_BUFFER,
    sizeof(Quad) * quads.offset,
    sizeof(Quad) * quads.size,
    quads.data
  );
  CHECK_OPENGL_ERROR();
  GL::unbindArrayBuffer();
}

inline void G2D::Renderer::render(const RenderJob &job) {
  vertArray.bind();
  program.use();
  
  GL::setUniform(viewProjLoc, job.viewProj);
  textures.at(job.tex).bind(0);
  GL::setUniform(colorLoc, job.color);
  
  program.validateAndLog();
  
  glDrawElements(
    GL_TRIANGLES,
    static_cast<GLsizei>(QUAD_INDICIES * (job.end - job.begin)),
    GL::TypeEnum<ElemType>::type,
    reinterpret_cast<GLvoid *>(QUAD_ELEM_SIZE * job.begin)
  );
  CHECK_OPENGL_ERROR();
  
  GL::unbindTexture2D(0);
  GL::unuseProgram();
  GL::unbindVertexArray();
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

inline void G2D::Renderer::fillIndicies(const size_t minQuads) {
  if (indicies.size() < minQuads * QUAD_INDICIES) {
    indicies.reserve(minQuads * QUAD_INDICIES);
    ElemType index = indicies.size() / QUAD_INDICIES * QUAD_VERTS;
    const ElemType lastIndex = minQuads * QUAD_VERTS;
    for (; index != lastIndex; index += QUAD_VERTS) {
      indicies.push_back(index + 0);
      indicies.push_back(index + 1);
      indicies.push_back(index + 2);
      indicies.push_back(index + 2);
      indicies.push_back(index + 3);
      indicies.push_back(index + 0);
    }
  }
}

inline void G2D::Renderer::setQuadBufSize(const size_t quads) {
  numQuads = quads;
  fillIndicies(numQuads);
  
  arrayBuf.bind();
  glBufferData(GL_ARRAY_BUFFER, numQuads * QUAD_ATTR_SIZE, nullptr, GL_DYNAMIC_DRAW);
  CHECK_OPENGL_ERROR();
  GL::unbindArrayBuffer();
  
  elemBuf.bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numQuads * QUAD_ELEM_SIZE, indicies.data(), GL_STATIC_DRAW);
  CHECK_OPENGL_ERROR();
  GL::unbindElementBuffer();
}
