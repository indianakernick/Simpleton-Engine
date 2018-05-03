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

namespace G2D {
  class QuadWriter {
  public:
    QuadWriter();
    
    /// Remove all of the sections
    void clear();
    
    /// Start a new section with the given rendering parameters
    void section(const RenderParams &);
    /// Make space for the given number of quads to avoid further reallocations
    void sectionSize(size_t);
    
    /// Start a new quad and return it
    Quad &quad();
    /// Start a new quad the is a duplicate of the previous quad and return it
    Quad &dup();
    
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
    
    /// Copy the quads into GPU memory and issue an number of draw calls
    void render(Renderer &) const;
    
  private:
    std::vector<Quad> quads;
    std::vector<size_t> sections;
    std::vector<RenderParams> params;
  };
}

#include "quad writer.inl"

#endif
