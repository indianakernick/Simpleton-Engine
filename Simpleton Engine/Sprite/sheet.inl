//
//  sheet.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include "../Memory/file io.hpp"
#include "../Utils/parse string.hpp"

inline Sprite::ID Sprite::Sheet::getIDfromName(const std::string_view name) const {
  auto iter = names.find(std::string(name));
  if (iter == names.cend()) {
    return NULL_SPRITE;
  } else {
    return iter->second;
  }
}

inline Sprite::Rect Sprite::Sheet::getSprite(const ID sprite) const {
  return rects.at(sprite);
}

inline bool Sprite::Sheet::hasWhitepixel() const {
  return whitepixel != NO_WHITEPIXEL;
}

inline glm::vec2 Sprite::Sheet::getWhitepixel() const {
  return whitepixel;
}

inline uint32_t Sprite::Sheet::getLength() const {
  return length;
}

inline Sprite::Sheet Sprite::makeSheet(const std::string_view atlasPath) try {
  const Memory::Buffer file = Memory::readFile(atlasPath);
  Utils::ParseString string(file.cdata<char>(), file.size());
  Sheet sheet;
  
  string.expect("{\"length\":");
  sheet.length = string.parseNumber<uint32_t>();
  string.expect(",\"names\":{");
  while (true) {
    string.expect('"');
    std::string name;
    string.copyUntil(name, '"');
    string.expect("\":");
    sheet.names.emplace(name, string.parseNumber<ID>());
    if (string.front() == '}') {
      break;
    } else {
      string.expect(',');
    }
  }
  
  string.expect("},\"rects\":[");
  while (true) {
    Rect rect;
    string.expect('[');
    string.parseNumber(rect.min.x);
    string.expect(',');
    string.parseNumber(rect.min.y);
    string.expect(',');
    string.parseNumber(rect.max.x);
    string.expect(',');
    string.parseNumber(rect.max.y);
    string.expect(']');
    
    rect.min /= glm::vec2(sheet.length);
    rect.max /= glm::vec2(sheet.length);
    sheet.rects.push_back(rect);
    
    if (string.front() == ']') {
      break;
    } else {
      string.expect(',');
    }
  }
  
  string.expect("]}\n");
  
  auto iter = sheet.names.find("__WHITEPIXEL__");
  if (iter != sheet.names.cend()) {
    sheet.names.erase(iter);
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
