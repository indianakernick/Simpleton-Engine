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
  enum class PlusXY {
    RIGHT_UP,
    LEFT_UP,
    RIGHT_DOWN,
    LEFT_DOWN
  };
  
  enum class Origin {
    TOP_LEFT,
    TOP_MID,
    TOP_RIGHT,
    MID_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_MID,
    BOTTOM_LEFT,
    MID_LEFT,
    CENTER
  };
  
  /// Get the depth of a depth enum. The last enumerator must be COUNT
  template <typename Enum>
  constexpr auto depth(const Enum e) {
    return static_cast<float>(e) / static_cast<float>(Enum::COUNT);
  }

  class QuadWriter {
  public:
    QuadWriter();
    
    /// Remove all of the sections
    void clear();
    
    /// Start a new section with the given rendering parameters
    void section(const RenderParams &);
    /// Make space for the given number of quads to avoid further reallocations
    void sectionSize(size_t);
    
    /// Sort the quads in the current section by the given sorting predicate
    template <typename Function>
    void sort(Function &&);
    
    /// Start a new quad and return it
    Quad &quad();
    /// Start a new quad the is a duplicate of the previous quad and return it
    Quad &dup();
    
    /// Set the depth of the current quad
    void depth(float);
    /// Set the depth of the current quad using an enum. The last enumerator
    /// must be COUNT
    template <typename Enum>
    void depth(Enum);
    
    /// Copy the positions of the verticies from the previous quad into the
    /// current quad
    void dupPos();
    /// Copy the positions of the verticies and the depth from the previous
    /// quad into the current quad
    void dupPosDepth();
    /// Write positions of verticies on the current quad as an axis-aligned
    /// rectangle.
    void tilePos(glm::vec2, glm::vec2 = {1.0f, 1.0f});
    /// Write positions of verticies on the current quad as a rectangle rotated
    /// around it's center. The quad is position relative to it's bottom left
    /// corner like tilePos.
    void rotTilePos(float, glm::vec2, glm::vec2 = {1.0f, 1.0f});
    /// Write positions of verticies on the current quad as rectangle rotated
    /// around a given origin. The quad is positioned relative to the origin.
    /// This function isn't quite as fast as rotTilePos without an origin.
    template <Origin ORIGIN>
    void rotTilePos(float, glm::vec2, glm::vec2 = {1.0f, 1.0f});
    
    /// Copy the texture coordinates of the verticies on the previous quad onto
    /// the current quad
    void dupTex();
    /// Write texture coordinates of vertices on the current quad assuming that
    /// the texture is sampled as an axis-aligned rectangle
    template <PlusXY PLUS_XY = PlusXY::RIGHT_UP>
    void tileTex(glm::vec2, glm::vec2);
    /// Write texture coordinates of vertices on the current quad assuming that
    /// the texture is sampled as an axis-aligned rectangle
    template <PlusXY PLUS_XY = PlusXY::RIGHT_UP>
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
