//
//  renderer.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 11/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_renderer_hpp
#define engine_graphics_2d_renderer_hpp

#include "types.hpp"
#include <string_view>
#include "surface.hpp"
#include "../OpenGL/buffer.hpp"
#include "../OpenGL/texture.hpp"
#include "../OpenGL/vertex array.hpp"
#include "../OpenGL/shader program.hpp"

namespace G2D {
  class Renderer {
  public:
    Renderer() = default;
  
    void initCore();
    void initES();
    void quit();
    
    TextureID addTexture(GL::Texture2D &&);
    TextureID addTexture(const Surface &, GL::TexParams2D);
    TextureID addTexture(std::string_view, GL::TexParams2D);
    
    void writeQuads(QuadRange, const Quad *);
    void render(QuadRange, const RenderJob &);
  
  private:
    std::vector<GL::Texture2D> textures;
    Elems indicies;
    size_t numQuads = 0;
    GL::ArrayBuffer arrayBuf;
    GL::ElementBuffer elemBuf;
    GL::VertexArray vertArray;
    GL::ShaderProgram program;
    GLint viewProjLoc;
    GLint texLoc;
    GLint colorLoc;
    
    void initState();
    void initUniforms();
    void initVertexArray();
    void fillIndicies(size_t);
    void setQuadBufSize(size_t);
  };
}

#include "renderer.inl"

#endif
