//
//  quad writer.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/4/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_quad_writer_hpp
#define engine_graphics_2d_quad_writer_hpp

#include "renderer.hpp"
#include "../Math/rect.hpp"
#include "../Memory/view.hpp"

namespace G2D {
  class QuadWriter {
  public:
    QuadWriter();
    
    /// Remove all of the sections
    void clear();
    
    /// Start a new section with the given rendering parameters
    void section(const RenderParams &);
    /// Make space for the given number of quads
    void sectionSize(size_t);
    
    /// Start a new quad and return it. Undefined behaviour if there isn't
    /// enough memory allocated.
    Quad &quad();
    /// Start a new quad and return it. Allocates memory if required. This check
    /// can reduce performance.
    Quad &quadAlloc();
    /// Start a new quad that is a duplicate of the previous quad and return it
    Quad &dup();
    /// Similar to dup() but makes a call to quadAlloc() rather than quad()
    Quad &dupAlloc();
    
    /// Set the depth of the current quad
    void depth(float);
    
    /// Copy the positions of the verticies from the previous quad into the
    /// current quad
    void dupPos();
    /// Copy the positions of the verticies and the depth from the previous
    /// quad into the current quad
    void dupPosDepth();
    /// Write positions of verticies on the current quad assuming that the quad
    /// is an axis-aligned rectangle
    void tilePos(glm::vec2, glm::vec2 = {1.0f, 1.0f});
    /// Write positions of verticies on the current quad assuming that the quad
    /// is a rectangle rotated around its center
    void rotTilePos(float, glm::vec2, glm::vec2 = {1.0f, 1.0f});
    
    /// Copy the texture coordinates of the verticies on the previous quad onto
    /// the current quad
    void dupTex();
    /// Write texture coordinates of vertices on the current quad assuming that
    /// the texture is sampled as an axis-aligned rectangle
    void tileTex(glm::vec2, glm::vec2);
    /// Write texture coordinates of vertices on the current quad assuming that
    /// the texture is sampled as an axis-aligned rectangle
    void tileTex(Math::RectPP<float>);
    
    /// Copy the quads into GPU memory and issue a number of draw calls
    void render(Renderer &) const;
    
  private:
    Quad *backQuad;
    size_t *backSection;
    RenderParams *backParam;
    Memory::View<Quad> quads;
    Memory::View<size_t> sections;
    Memory::View<RenderParams> params;
    
    bool hasQuad() const;
    bool hasQuads() const;
    bool hasSection() const;
  };
}

#include "fast quad writer.inl"

#endif
