//
//  quad writer.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/4/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

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

inline void G2D::QuadWriter::tilePos(
  const float depth,
  const glm::vec2 pos,
  const glm::vec2 size
) {
  assert(quads.size() && sections.size() && sections.back());
  
  Quad &quad = quads.back();
  quad[0].pos = {pos.x, pos.y, depth};
  quad[1].pos = {pos.x + size.x, pos.y, depth};
  quad[2].pos = {pos.x + size.x, pos.y + size.y, depth};
  quad[3].pos = {pos.x, pos.y + size.y, depth};
}

inline void G2D::QuadWriter::tileTex(const glm::vec2 min, const glm::vec2 max) {
  assert(quads.size() && sections.size() && sections.back());
  
  Quad &quad = quads.back();
  quad[0].texCoord = min;
  quad[1].texCoord = {max.x, min.y};
  quad[2].texCoord = max;
  quad[3].texCoord = {min.x, max.y};
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
