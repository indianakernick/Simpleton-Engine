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
#include "types.hpp"
#include <string_view>
#include <unordered_map>

namespace Sprite {
  class Sheet {
  public:
    //only the factory function can make spritesheets
    friend Sheet makeSheet(std::string_view);
    
    Sheet() = default;
    ~Sheet() = default;
    Sheet(Sheet &&) = default;
    Sheet &operator=(Sheet &&) = default;
    
    /// Returns NULL_SPRITE if the name is invalid
    ID getIDfromName(std::string_view) const;
    /// Throws std::out_of_range if SpriteID is invalid
    Rect getSprite(ID) const;
    /// Is there a whitepixel position?
    bool hasWhitepixel() const;
    /// Get whitepixel pos. Returns NO_WHITEPIXEL if there isn't one
    glm::vec2 getWhitepixel() const;
    
  private:
    std::unordered_map<std::string, ID> names;
    std::vector<Rect> rects;
    glm::vec2 whitepixel;
  };
  
  Sheet makeSheet(std::string_view);
}

#include "sheet.inl"

#endif
