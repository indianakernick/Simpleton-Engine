//
//  quad writer.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/4/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include <glm/gtc/constants.hpp>

inline G2D::QuadWriter::QuadWriter() {
  quads.reserve(2048);
  sections.reserve(64);
  params.reserve(64);
}

inline void G2D::QuadWriter::clear() {
  quads.clear();
  sections.clear();
  params.clear();
}

inline void G2D::QuadWriter::section(const RenderParams &param) {
  sections.push_back(quads.size());
  params.push_back(param);
}

inline void G2D::QuadWriter::sectionSize(const size_t size) {
  quads.reserve(quads.size() + size);
}

inline G2D::Quad &G2D::QuadWriter::quad() {
  assert(sections.size());
  
  quads.push_back({});
  return quads.back();
}

inline G2D::Quad &G2D::QuadWriter::dup() {
  assert(sections.size() && quads.size());
  
  quads.push_back(quads.back());
  return quads.back();
}

inline void G2D::QuadWriter::depth(const float depth) {
  assert(quads.size() && sections.size());
  
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

inline void G2D::QuadWriter::dupPos() {
  assert(quads.size() > 1 && sections.size());
  
  Quad &quad = quads.back();
  const Quad &prev = *(quads.cend() - 2);
  for (size_t i = 0; i != 4; ++i) {
    detail::setPos(quad[i].pos, prev[i].pos);
  }
}

inline void G2D::QuadWriter::dupPosDepth() {
  assert(quads.size() > 1 && sections.size());
  
  Quad &quad = quads.back();
  const Quad &prev = *(quads.cend() - 2);
  for (size_t i = 0; i != 4; ++i) {
    quad[i].pos = prev[i].pos;
  }
}

inline void G2D::QuadWriter::tilePos(
  const glm::vec2 pos,
  const glm::vec2 size
) {
  assert(quads.size() && sections.size());
  
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
  assert(quads.size() && sections.size());

  const glm::vec2 halfSize = size * 0.5f;
  const glm::vec2 topRight = glm::vec2(
    std::cos(angle + glm::quarter_pi<float>()),
    std::sin(angle + glm::quarter_pi<float>())
  ) * halfSize * glm::root_two<float>();
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

template <G2D::Origin ORIGIN>
void G2D::QuadWriter::rotTilePos(
  const float angle,
  const glm::vec2 pos,
  const glm::vec2 size
) {
  assert(quads.size() && sections.size());
  
  constexpr glm::vec2 ORIGIN_POS[9] = {
    {0.5f, -0.5f}, {0.0f, -0.5f}, {-0.5f, -0.5f}, {-0.5f, 0.0f},
    {-0.5f, 0.5f}, {0.0f, 0.5f}, {0.5f, 0.5f}, {0.5f, 0.0f},
    {0.0f, 0.0f}
  };
  constexpr glm::vec2 originPos = ORIGIN_POS[static_cast<size_t>(ORIGIN)];
  
  const glm::vec2 origin = originPos * size;
  
  const float c = std::cos(angle);
  const float s = std::sin(angle);
  const glm::mat2 rot = {
    {c, s},
    {-s, c}
  };
  
  const glm::vec2 halfSize = size * 0.5f;
  const glm::vec2 tr = halfSize;
  const glm::vec2 bl = -halfSize;
  const glm::vec2 tl = {bl.x, tr.y};
  const glm::vec2 br = {tr.x, bl.y};
  
  Quad &quad = quads.back();
  detail::setPos(quad[0].pos, pos + rot * (bl + origin));
  detail::setPos(quad[1].pos, pos + rot * (br + origin));
  detail::setPos(quad[2].pos, pos + rot * (tr + origin));
  detail::setPos(quad[3].pos, pos + rot * (tl + origin));
}

inline void G2D::QuadWriter::dupTex() {
  assert(quads.size() > 1 && sections.size());
  
  Quad &quad = quads.back();
  const Quad &prev = *(quads.cend() - 2);
  for (size_t i = 0; i != 4; ++i) {
    quad[i].texCoord = prev[i].texCoord;
  }
}

template <G2D::PlusXY PLUS_XY>
void G2D::QuadWriter::tileTex(const glm::vec2 min, const glm::vec2 max) {
  assert(quads.size() && sections.size());
  
  constexpr size_t Is[4][4] = {
                  // +x      +y
    {0, 1, 2, 3}, // right   up
    {1, 0, 3, 2}, // left    up
    {3, 2, 1, 0}, // right   down
    {2, 3, 0, 1}  // left    down
  };
  constexpr size_t i = static_cast<size_t>(PLUS_XY);
  
  Quad &quad = quads.back();
  quad[Is[i][0]].texCoord = min;
  quad[Is[i][1]].texCoord = {max.x, min.y};
  quad[Is[i][2]].texCoord = max;
  quad[Is[i][3]].texCoord = {min.x, max.y};
}

template <G2D::PlusXY PLUS_XY>
void G2D::QuadWriter::tileTex(const Math::RectPP<float> coords) {
  tileTex<PLUS_XY>(coords.min, coords.max);
}

inline void G2D::QuadWriter::render(Renderer &renderer) const {
  renderer.writeQuads({0, quads.size()}, quads.data());
  if (sections.empty()) {
    return;
  }
  QuadRange range;
  range.begin = sections[0];
  for (size_t s = 1; s != sections.size(); ++s) {
    range.end = sections[s];
    renderer.render(range, params[s - 1]);
    range.begin = range.end;
  }
  range.end = quads.size();
  renderer.render(range, params.back());
}
