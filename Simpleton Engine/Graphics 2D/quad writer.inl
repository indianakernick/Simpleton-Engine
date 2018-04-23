//
//  quad writer.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/4/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include <glm/gtc/constants.hpp>

inline void G2D::QuadWriter::clear() {
  quads.clear();
  sections.clear();
  params.clear();
}

inline void G2D::QuadWriter::section(const RenderParams &param) {
  sections.push_back(0);
  params.push_back(param);
}

inline void G2D::QuadWriter::sectionSize(const size_t size) {
  quads.reserve(quads.size() + size);
}

inline G2D::Quad &G2D::QuadWriter::quad() {
  assert(sections.size());
  
  ++sections.back();
  quads.push_back({});
  return quads.back();
}

inline void G2D::QuadWriter::depth(const float depth) {
  assert(quads.size() && sections.size() && sections.back());
  
  Quad &quad = quads.back();
  quad[0].pos.z =
  quad[1].pos.z =
  quad[2].pos.z =
  quad[3].pos.z = depth;
}

namespace G2D::detail {
  inline void setPos(glm::vec3 &dst, const glm::vec2 src) {
    dst.x = src.x;
    dst.y = src.y;
  }
}

inline void G2D::QuadWriter::tilePos(
  const glm::vec2 pos,
  const glm::vec2 size
) {
  assert(quads.size() && sections.size() && sections.back());
  
  Quad &quad = quads.back();
  detail::setPos(quad[0].pos, pos);
  detail::setPos(quad[1].pos, {pos.x + size.x, pos.y});
  detail::setPos(quad[2].pos, pos + size);
  detail::setPos(quad[3].pos, {pos.x, pos.y + size.y});
}

inline void G2D::QuadWriter::rotTilePos(
  const float angle,
  const glm::vec2 pos,
  const glm::vec2 size
) {
  assert(quads.size() && sections.size() && sections.back());

  const glm::vec2 halfSize = size * 0.5f;
  const glm::vec2 topRight = glm::vec2(
    std::cos(angle + glm::quarter_pi<float>()),
    std::sin(angle + glm::quarter_pi<float>())
  ) * halfSize;
  const glm::vec2 topLeft = {-topRight.y, topRight.x};
  const glm::vec2 botLeft = -topRight;
  const glm::vec2 botRight = -topLeft;
  const glm::vec2 shift = pos + halfSize;
  
  Quad &quad = quads.back();
  detail::setPos(quad[0].pos, botLeft + shift);
  detail::setPos(quad[1].pos, botRight + shift);
  detail::setPos(quad[2].pos, topRight + shift);
  detail::setPos(quad[3].pos, topLeft + shift);
}

inline void G2D::QuadWriter::tileTex(const glm::vec2 min, const glm::vec2 max) {
  assert(quads.size() && sections.size() && sections.back());
  
  Quad &quad = quads.back();
  quad[0].texCoord = min;
  quad[1].texCoord = {max.x, min.y};
  quad[2].texCoord = max;
  quad[3].texCoord = {min.x, max.y};
}

inline void G2D::QuadWriter::tileTex(const Math::RectPP<float> coords) {
  tileTex(coords.min, coords.max);
}

inline void G2D::QuadWriter::render(Renderer &renderer) const {
  renderer.writeQuads({0, quads.size()}, quads.data());
  size_t count = 0;
  for (size_t s = 0; s != sections.size(); ++s) {
    QuadRange range;
    range.begin = count;
    range.end = (count += sections[s]);
    renderer.render(range, params[s]);
  }
}
