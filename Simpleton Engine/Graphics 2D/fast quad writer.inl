//
//  quad writer.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/4/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include <glm/gtc/constants.hpp>

inline G2D::QuadWriter::QuadWriter()
  : quads(2048), sections(64) {
  backQuad = quads.data() - 1;
  backSection = sections.data() - 1;
  params.reserve(64);
}

inline void G2D::QuadWriter::clear() {
  backQuad = quads.data() - 1;
  backSection = sections.data() - 1;
  params.clear();
}

inline void G2D::QuadWriter::section(const RenderParams &param) {
  *(++backSection) = backQuad;
  params.push_back(param);
}

inline void G2D::QuadWriter::sectionSize(const size_t size) {
  // number of quads minus 1
  const size_t numQuadsM1 = backQuad - quads.data();
  if (quads.size() <= numQuadsM1 + size) {
    
  }
}

inline G2D::Quad &G2D::QuadWriter::quad() {
  assert(hasSection());
  return *(++backQuad);
}

inline G2D::Quad &G2D::QuadWriter::dup() {
  assert(hasQuad());
  return quad() = *backQuad;
}

inline void G2D::QuadWriter::depth(const float depth) {
  assert(hasQuad());
  Quad &quad = *backQuad;
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

inline void G2D::QuadWriter::dupPos() {
  assert(hasQuads());
  Quad &quad = *backQuad;
  Quad &prev = *(backQuad - 1);
  for (size_t i = 0; i != 4; ++i) {
    detail::setPos(quad[i].pos, prev[i].pos);
  }
}

inline void G2D::QuadWriter::dupPosDepth() {
  assert(hasQuads());
  Quad &quad = *backQuad;
  Quad &prev = *(backQuad - 1);
  for (size_t i = 0; i != 4; ++i) {
    quad[i].pos = prev[i].pos;
  }
}

inline void G2D::QuadWriter::tilePos(
  const glm::vec2 pos,
  const glm::vec2 size
) {
  assert(hasQuad());
  
  Quad &quad = *backQuad;
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
  assert(hasQuad());

  const glm::vec2 halfSize = size * 0.5f;
  const glm::vec2 topRight = glm::vec2(
    std::cos(angle + glm::quarter_pi<float>()),
    std::sin(angle + glm::quarter_pi<float>())
  ) * halfSize;
  const glm::vec2 topLeft = {-topRight.y, topRight.x};
  const glm::vec2 botLeft = -topRight;
  const glm::vec2 botRight = -topLeft;
  const glm::vec2 shift = pos + halfSize;
  
  Quad &quad = *backQuad;
  detail::setPos(quad[0].pos, botLeft + shift);
  detail::setPos(quad[1].pos, botRight + shift);
  detail::setPos(quad[2].pos, topRight + shift);
  detail::setPos(quad[3].pos, topLeft + shift);
}

inline void G2D::QuadWriter::dupTex() {
  assert(hasQuads());
  
  Quad &quad = *backQuad;
  Quad &prev = *(backQuad - 1);
  for (size_t i = 0; i != 4; ++i) {
    quad[i].texCoord = prev[i].texCoord;
  }
}

inline void G2D::QuadWriter::tileTex(const glm::vec2 min, const glm::vec2 max) {
  assert(hasQuad());
  
  Quad &quad = *backQuad;
  quad[0].texCoord = min;
  quad[1].texCoord = {max.x, min.y};
  quad[2].texCoord = max;
  quad[3].texCoord = {min.x, max.y};
}

inline void G2D::QuadWriter::tileTex(const Math::RectPP<float> coords) {
  tileTex(coords.min, coords.max);
}

inline void G2D::QuadWriter::render(Renderer &renderer) const {
  renderer.writeQuads({
    0, static_cast<size_t>(1 + backQuad - quads.data())},
    quads.data()
  );
  const size_t numSections = 1 + backSection - sections.data();
  for (size_t s = 0; s != numSections; ++s) {
    QuadRange range;
    range.begin = 1 + sections[s] - quads.data();
    if (s == static_cast<size_t>(backSection - sections.data())) {
      range.end = 1 + *backSection - quads.data();
    } else {
      range.end = 1 + sections[s + 1] - quads.data();
    }
    renderer.render(range, params[s]);
  }
}

inline bool G2D::QuadWriter::hasQuad() const {
  return backQuad >= quads.data();
}

inline bool G2D::QuadWriter::hasQuads() const {
  return backQuad > quads.data();
}

inline bool G2D::QuadWriter::hasSection() const {
  return backSection >= sections.data();
}
