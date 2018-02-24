//
//  types.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 24/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_unpack_types_hpp
#define engine_unpack_types_hpp

#include "../Math/rect.hpp"

namespace Unpack {
  using Rect = Math::RectPP<float>;
  
  class AtlasReadError final : public std::runtime_error {
  public:
    explicit AtlasReadError(const std::exception &);
  };
  
  using SpriteID = uint32_t;
  constexpr SpriteID NULL_SPRITE = std::numeric_limits<SpriteID>::max();
  constexpr glm::vec2 NO_WHITEPIXEL = {-1.0f, -1.0f};
}

#include "types.inl"

#endif
