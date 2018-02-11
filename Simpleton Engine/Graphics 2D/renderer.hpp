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
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <Simpleton/OpenGL/buffer.hpp>
#include <Simpleton/OpenGL/texture.hpp>
#include <Simpleton/OpenGL/vertex array.hpp>
#include <Simpleton/OpenGL/shader program.hpp>

namespace G2D {
  class Renderer {
  public:
    Renderer() = default;
  
    void init();
    void quit();
    
    TextureID addTexture(std::string_view);
    TextureID addTexture(GL::Texture2D &&);
    
    QuadIter getQuadBuf();
    size_t getQuadBufSize();
    
    void setQuadBufSize(size_t);
    void render(const glm::mat3 &, TextureID, glm::vec4 = {1.0f, 1.0f, 1.0f, 1.0f});
  
  private:
    std::vector<GL::Texture2D> textures;
    GL::ArrayBuffer arrayBuf;
    GL::ElementBuffer elemBuf;
    Quads quads;
    Elems indicies;
    
    static GL::VertexArray vertArray;
    static GL::ShaderProgram program;
    static GLint viewProjLoc;
    static GLint texLoc;
    static GLint colorLoc;
  };
}

#include "renderer.inl"

#endif