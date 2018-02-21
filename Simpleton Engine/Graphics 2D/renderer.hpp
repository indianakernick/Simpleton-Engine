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
    
    Quad *getQuadBuf();
    size_t getQuadBufSize() const;
    
    void setQuadBufSize(size_t);
    void render(const glm::mat3 &, TextureID, glm::vec4 = glm::vec4(1.0f));
  
  private:
    std::vector<GL::Texture2D> textures;
    Quads quads;
    Elems indicies;
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
    void writeVerticies();
  };
}

#include "renderer.inl"

#endif
