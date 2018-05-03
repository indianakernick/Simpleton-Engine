//
//  fast quad writer.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 30/4/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include <glm/gtc/constants.hpp>

inline G2D::FastQuadWriter::FastQuadWriter()
  : quads(2048), sections(64), params(64) {
  clear();
}

inline void G2D::FastQuadWriter::clear() {
  backQuad = quads.data() - 1;
  backSection = sections.data() - 1;
  backParam = params.data() - 1;
}

inline void G2D::FastQuadWriter::section(const RenderParams &param) {
  ++backSection;
  ++backParam;
  
  if (backSection == sections.end()) {
    const size_t oldSize = sections.size();
    const size_t newSize = oldSize * 2;
    sections.resizeCopy(newSize);
    params.resizeCopy(newSize);
    backSection = sections.data() + oldSize;
    backParam = params.data() + oldSize;
  }
  
  *backSection = backQuad - quads.data();
  *backParam = param;
}

inline void G2D::FastQuadWriter::sectionSize(const size_t size) {
  // number of quads minus 1
  const size_t backIndex = backQuad - quads.data();
  const size_t numQuadsM1 = backIndex + size;
  if (quads.size() <= numQuadsM1) {
    quads.resizeCopy(
      // Ceil to nearest power of two
      (1 << (Utils::bits<long long> - __builtin_clzll(numQuadsM1)))
    );
    backQuad = quads.data() + backIndex;
  }
}

inline G2D::Quad &G2D::FastQuadWriter::quad() {
  assert(hasSection());
  return *(++backQuad);
}

inline G2D::Quad &G2D::FastQuadWriter::quadAlloc() {
  assert(hasSection());
  ++backQuad;
  const size_t oldSize = quads.size();
  if (backQuad == quads.data() + oldSize) {
    quads.resizeCopy(oldSize * 2);
    backQuad = quads.data() + oldSize;
  }
  return *backQuad;
}

inline G2D::Quad &G2D::FastQuadWriter::dup() {
  assert(hasQuad());
  return quad() = *backQuad;
}

inline G2D::Quad &G2D::FastQuadWriter::dupAlloc() {
  assert(hasQuad());
  return quadAlloc() = *backQuad;
}

inline void G2D::FastQuadWriter::depth(const float depth) {
  assert(hasQuad());
  Quad &quad = *backQuad;
  quad[0].pos.z =
  quad[1].pos.z =
  quad[2].pos.z =
  quad[3].pos.z = depth;
}

namespace G2D::detail {
  inline void setPosF(glm::vec3 &dst, const glm::vec2 src) {
    dst.x = src.x;
    dst.y = src.y;
  }
}

inline void G2D::FastQuadWriter::dupPos() {
  assert(hasQuads());
  Quad &quad = *backQuad;
  Quad &prev = *(backQuad - 1);
  for (size_t i = 0; i != 4; ++i) {
    detail::setPosF(quad[i].pos, prev[i].pos);
  }
}

inline void G2D::FastQuadWriter::dupPosDepth() {
  assert(hasQuads());
  Quad &quad = *backQuad;
  Quad &prev = *(backQuad - 1);
  for (size_t i = 0; i != 4; ++i) {
    quad[i].pos = prev[i].pos;
  }
}

inline void G2D::FastQuadWriter::tilePos(
  const glm::vec2 pos,
  const glm::vec2 size
) {
  assert(hasQuad());
  
  Quad &quad = *backQuad;
  detail::setPosF(quad[0].pos, pos);
  detail::setPosF(quad[1].pos, {pos.x + size.x, pos.y});
  detail::setPosF(quad[2].pos, pos + size);
  detail::setPosF(quad[3].pos, {pos.x, pos.y + size.y});
}

inline void G2D::FastQuadWriter::rotTilePos(
  const float angle,
  const glm::vec2 pos,
  const glm::vec2 size
) {
  assert(hasQuad());

  const glm::vec2 halfSize = size * 0.5f;
  const glm::vec2 topRight = glm::vec2(
    glm::root_two<float>() * std::cos(angle + glm::quarter_pi<float>()),
    glm::root_two<float>() * std::sin(angle + glm::quarter_pi<float>())
  ) * halfSize;
  const glm::vec2 topLeft = {-topRight.y, topRight.x};
  const glm::vec2 botLeft = -topRight;
  const glm::vec2 botRight = -topLeft;
  const glm::vec2 shift = pos + halfSize;
  
  Quad &quad = *backQuad;
  detail::setPosF(quad[0].pos, botLeft + shift);
  detail::setPosF(quad[1].pos, botRight + shift);
  detail::setPosF(quad[2].pos, topRight + shift);
  detail::setPosF(quad[3].pos, topLeft + shift);
}

inline void G2D::FastQuadWriter::dupTex() {
  assert(hasQuads());
  
  Quad &quad = *backQuad;
  Quad &prev = *(backQuad - 1);
  for (size_t i = 0; i != 4; ++i) {
    quad[i].texCoord = prev[i].texCoord;
  }
}

inline void G2D::FastQuadWriter::tileTex(const glm::vec2 min, const glm::vec2 max) {
  assert(hasQuad());
  
  Quad &quad = *backQuad;
  quad[0].texCoord = min;
  quad[1].texCoord = {max.x, min.y};
  quad[2].texCoord = max;
  quad[3].texCoord = {min.x, max.y};
}

inline void G2D::FastQuadWriter::tileTex(const Math::RectPP<float> coords) {
  tileTex(coords.min, coords.max);
}

inline void G2D::FastQuadWriter::render(Renderer &renderer) const {
  // the indicies in the sections array point to the beginnings of each
  // section minus 1. backQuad points to the last quad of the last section
  const size_t numQuads = 1 + backQuad - quads.data();
  renderer.writeQuads({0, numQuads}, quads.data());
  size_t *const endSection = 1 + backSection;
  const size_t *section = sections.data();
  if (section == endSection) {
    return;
  }
  const RenderParams *param = params.data();
  QuadRange range;
  range.begin = 1 + *section;
  ++section;
  for (; section != endSection; ++section, ++param) {
    range.end = 1 + *section;
    renderer.render(range, *param);
    range.begin = range.end;
  }
  range.end = 1 + backQuad - quads.data();
  renderer.render(range, *param);
}

inline bool G2D::FastQuadWriter::hasQuad() const {
  return backQuad >= quads.data();
}

inline bool G2D::FastQuadWriter::hasQuads() const {
  return backQuad > quads.data();
}

inline bool G2D::FastQuadWriter::hasSection() const {
  return backSection >= sections.data();
}
