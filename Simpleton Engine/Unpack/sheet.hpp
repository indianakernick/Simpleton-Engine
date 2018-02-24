//
//  sheet.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_unpack_sheet_hpp
#define engine_unpack_sheet_hpp

#include <string>
#include <vector>
#include <string_view>
#include <unordered_map>
#include "../Math/rect.hpp"

namespace Unpack {
  using Rect = Math::RectPP<float>;
  
  class AtlasReadError final : public std::runtime_error {
  public:
    explicit AtlasReadError(const char *);
    explicit AtlasReadError(const std::exception &);
  };
  
  using SpriteID = uint32_t;
  constexpr SpriteID NULL_SPRITE = std::numeric_limits<SpriteID>::max();
  constexpr glm::vec2 NO_WHITEPIXEL = {-1.0f, -1.0f};
  
  class Spritesheet {
  public:
    //only the factory function can make spritesheets
    friend Spritesheet makeSpritesheet(std::string_view);
    
    Spritesheet() = default;
    ~Spritesheet() = default;
    Spritesheet(Spritesheet &&) = default;
    Spritesheet &operator=(Spritesheet &&) = default;
    
    /// Returns NULL_SPRITE if the name is invalid
    SpriteID getIDfromName(std::string_view) const;
    /// Throws std::out_of_range if SpriteID is invalid
    Rect getSprite(SpriteID) const;
    /// Is there a whitepixel position?
    bool hasWhitepixel() const;
    /// Get whitepixel pos. Returns NO_WHITEPIXEL if there isn't one
    glm::vec2 getWhitepixel() const;
    
  private:
    std::unordered_map<std::string, SpriteID> names;
    std::vector<Rect> rects;
    glm::vec2 whitepixel;
  };
  
  Spritesheet makeSpritesheet(std::string_view);
}

#include "sheet.inl"

#endif
