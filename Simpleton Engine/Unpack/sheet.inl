//
//  sheet.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include "../Memory/file io.hpp"
#include "../Utils/parse string.hpp"

inline Unpack::SpriteID Unpack::Spritesheet::getIDfromName(const std::string_view name) const {
  auto iter = names.find(std::string(name));
  if (iter == names.cend()) {
    return NULL_SPRITE;
  } else {
    return iter->second;
  }
}

inline Unpack::Rect Unpack::Spritesheet::getSprite(const SpriteID sprite) const {
  return rects.at(sprite);
}

inline bool Unpack::Spritesheet::hasWhitepixel() const {
  return whitepixel != NO_WHITEPIXEL;
}

inline glm::vec2 Unpack::Spritesheet::getWhitepixel() const {
  return whitepixel;
}

inline Unpack::Spritesheet Unpack::makeSpritesheet(const std::string_view atlasPath) try {
  const Memory::Buffer file = Memory::readFile(atlasPath);
  Utils::ParseString string(file.cdata<char>(), file.size());
  Spritesheet sheet;
  
  uint32_t numSprites;
  string.readNumberLil(numSprites);
  
  sheet.names.reserve(numSprites);
  sheet.rects.reserve(numSprites);
  
  for (SpriteID i = 0; i != numSprites - 1; ++i) {
    std::string name;
    string.copyUntil(name, '\n');
    string.expect('\n');
    sheet.names.emplace(name, i);
  }
  
  bool hasWhitepixel = false;
  std::string lastName;
  string.copyUntil(lastName, '\n');
  string.expect('\n');
  if (lastName == "__WHITEPIXEL__") {
    hasWhitepixel = true;
  } else {
    sheet.names.emplace(lastName, numSprites - 1);
  }
  
  for (uint32_t i = 0; i != numSprites; ++i) {
    Rect rect;
    string.readNumbersLil(&rect.min.x, 4);
    sheet.rects.push_back(rect);
  }
  
  if (hasWhitepixel) {
    const Rect whiteRect = sheet.rects.back();
    sheet.rects.pop_back();
    sheet.whitepixel = {
      (whiteRect.min.x + whiteRect.max.x) / 2.0f,
      (whiteRect.min.y + whiteRect.max.y) / 2.0f
    };
  } else {
    sheet.whitepixel = NO_WHITEPIXEL;
  }
  
  return sheet;
} catch (std::exception &e) {
  throw AtlasReadError(e);
}
