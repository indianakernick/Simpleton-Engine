//
//  transform.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 13/9/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_grid_transform_hpp
#define engine_grid_transform_hpp

#include "grid.hpp"

#include "transform.inl"

namespace Grid {
  namespace detail {
    template <Coord Width, Coord Height>
    struct NewDims {
      static constexpr Coord new_width = Width;
      static constexpr Coord new_height = Height;
    };
  }
  
  template <Coord Width, Coord Height>
  struct FlipX : detail::NewDims<Width, Height> {
    static constexpr Pos apply(const Pos pos) {
      return {Width - pos.x - 1, pos.y};
    }
  };
  
  template <Coord Width, Coord Height>
  struct FlipY : detail::NewDims<Width, Height> {
    static constexpr Pos apply(const Pos pos) {
      return {pos.x, Height - pos.y - 1};
    }
  };
  
  template <Coord Width, Coord Height>
  struct Transpose : detail::NewDims<Height, Width> {
    static constexpr Pos apply(const Pos pos) {
      return {pos.y, pos.x};
    }
  };
  
  template <Coord Width, Coord Height>
  using FlipXY = Combine<FlipX, FlipY>::template type<Width, Height>;
  
  template <Coord Width, Coord Height>
  using RotX2Y = Combine<FlipX, Transpose>::template type<Width, Height>;
  
  template <Coord Width, Coord Height>
  using RotY2X = Combine<FlipY, Transpose>::template type<Width, Height>;
  
  template <template <Coord, Coord> typename... Transformers, typename Tile, Coord Width, Coord Height>
  auto transform(const Grid<Tile, Width, Height> &in) {
    using Trans = typename Combine<Transformers...>::template type<Width, Height>;
    Grid<Tile, Trans::new_width, Trans::new_height> out;
    for (const Coord y : in.vert()) {
      for (const Coord x : in.hori()) {
        out[Trans::apply({x, y})] = in(x, y);
      }
    }
    return out;
  }
}

#endif
