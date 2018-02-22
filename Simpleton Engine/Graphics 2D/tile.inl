//
//  tile.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 21/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

inline void G2D::tilePos(
  Quad &quad,
  const float depth,
  const glm::vec2 pos,
  const glm::vec2 size
) {
  quad[0].pos = {pos.x, pos.y, depth};
  quad[1].pos = {pos.x + size.x, pos.y, depth};
  quad[2].pos = {pos.x + size.x, pos.y + size.y, depth};
  quad[3].pos = {pos.x, pos.y + size.y, depth};
}

inline void G2D::tileTex(
  Quad &quad,
  const TexCoordType min,
  const TexCoordType max
) {
  quad[0].texCoord = min;
  quad[1].texCoord = {max.x, min.y};
  quad[2].texCoord = max;
  quad[3].texCoord = {min.x, max.y};
}
