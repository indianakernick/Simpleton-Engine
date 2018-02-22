//
//  sheet.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include <fstream>
#include "../Data/json.hpp"

inline Unpack::AtlasReadError::AtlasReadError(const char *const msg)
  : std::runtime_error(std::string("Atlas read error: ") + msg) {}

inline Unpack::AtlasReadError::AtlasReadError(const std::exception &exception)
  : std::runtime_error(exception.what()) {}

inline Unpack::SpriteID Unpack::Spritesheet::getIDfromName(const std::string_view name) const {
  auto iter = spriteNames.find(std::string(name));
  if (iter == spriteNames.cend()) {
    return NULL_SPRITE;
  } else {
    return iter->second;
  }
}

inline Unpack::Rect Unpack::Spritesheet::getSprite(const SpriteID sprite) const {
  return sprites.at(sprite);
}

inline bool Unpack::Spritesheet::hasWhitepixel() const {
  return whitepixel.x != NO_WHITEPIXEL.x || whitepixel.y != NO_WHITEPIXEL.y;
}

inline glm::vec2 Unpack::Spritesheet::getWhitepixel() const {
  return whitepixel;
}

inline glm::ivec2 Unpack::Spritesheet::getSize() const {
  return size;
}

inline Unpack::Spritesheet Unpack::makeSpritesheet(const std::string_view atlasPath) try {
  std::ifstream atlasFile(atlasPath.data(), std::fstream::binary);
  if (!atlasFile.is_open()) {
    throw AtlasReadError("Failed to open file");
  }
  nlohmann::json doc;
  atlasFile >> doc;
  Spritesheet sheet;
  
  sheet.size = doc.at("size").get<glm::ivec2>();
  if (sheet.size.x < 1 || sheet.size.y < 1) {
    throw AtlasReadError("Size is out of range");
  }
  
  if (const auto whitepixelIter = doc.find("whitepixel"); whitepixelIter != doc.end()) {
    sheet.whitepixel = whitepixelIter->get<glm::vec2>();
    if (sheet.whitepixel.x < 0 || sheet.whitepixel.y < 0) {
      throw AtlasReadError("Whitepixel is out of range");
    }
    sheet.whitepixel.y = sheet.size.y - sheet.whitepixel.y;
    sheet.whitepixel += glm::vec2(0.5f);
    sheet.whitepixel /= sheet.size;
  } else {
    sheet.whitepixel = NO_WHITEPIXEL;
  }
  
  const json &sprites = doc.at("rects");
  for (auto s = sprites.cbegin(); s != sprites.cend(); ++s) {
    int rect[4] = {s->at(0), s->at(1), s->at(2), s->at(3)};
    if (
      rect[0] < 0 ||
      rect[1] < 0 ||
      rect[2] < 1 ||
      rect[3] < 1 ||
      rect[0] + rect[2] > sheet.size.x ||
      rect[1] + rect[3] > sheet.size.y
    ) {
      throw AtlasReadError("Rectangle is out of range");
    }
    
    // Cast RectPS<int, Math::Dir::RIGHT, Math::Dir::DOWN>
    // to   RectPP<float, Math::Dir::RIGHT, Math::Dir::UP>
    
    rect[1] = sheet.size.y - rect[1] - rect[3];
    rect[2] += rect[0];
    rect[3] += rect[1];
    Rect floatRect(rect[0], rect[1], rect[2], rect[3]);
    floatRect.min /= sheet.size;
    floatRect.max /= sheet.size;
    sheet.sprites.push_back(floatRect);
  }
  
  sheet.spriteNames = doc.at("names").get<decltype(sheet.spriteNames)>();
  
  if (sheet.sprites.size() != sheet.spriteNames.size()) {
    throw AtlasReadError("There must be one name for each rectangle");
  }
  
  return sheet;
} catch (AtlasReadError &) {
  throw;
} catch (std::exception &e) {
  throw AtlasReadError(e);
}
