//
//  types.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 11/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_types_hpp
#define engine_graphics_2d_types_hpp

#include <array>
#include <tuple>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>

namespace G2D {
  using PosType = glm::vec3;
  using TexCoordType = glm::vec2;
  using ElemType = uint16_t;
  
  struct Vertex {
    PosType pos;
    TexCoordType texCoord;
  };
  
  using Quad = std::array<Vertex, 4>;
  using Elems = std::vector<ElemType>;
  using TextureID = size_t;
  
  struct QuadRange {
    size_t begin;
    size_t end;
    
    size_t size() const {
      return end - begin;
    }
  };
  
  struct RenderJob {
    glm::mat3 viewProj = {};
    TextureID tex = 0;
    glm::vec4 color = glm::vec4(1.0f);
  };
  
  constexpr size_t QUAD_INDICIES = 6;
  constexpr size_t QUAD_VERTS = 4;
  constexpr size_t QUAD_ATTR_SIZE = sizeof(Quad);
  constexpr size_t QUAD_ELEM_SIZE = sizeof(ElemType) * QUAD_INDICIES;
  
  using Attribs = std::tuple<PosType, TexCoordType>;
}

#endif
