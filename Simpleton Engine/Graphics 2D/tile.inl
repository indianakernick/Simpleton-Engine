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
  const TexCoordType pos,
  const TexCoordType size
) {
  quad[0].texCoord = pos;
  quad[1].texCoord = {pos.x + size.x, pos.y};
  quad[2].texCoord = {pos.x + size.x, pos.y + size.y};
  quad[3].texCoord = {pos.x, pos.y + size.y};
}
