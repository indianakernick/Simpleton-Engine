//
//  blit.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 13/9/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

template <
  typename Tile,
  Grid::Coord DstWidth,
  Grid::Coord DstHeight,
  Grid::Coord SrcWidth,
  Grid::Coord SrcHeight
>
void Grid::blit(
  Grid<Tile, DstWidth, DstHeight> &dst,
  const Grid<Tile, SrcWidth, SrcHeight> &src,
  const Pos pos
) {
  const Coord width = std::min(src.width(), dst.width() - pos.x);
  const Coord height = std::min(src.height(), dst.height() - pos.y);
  for (const Coord y : Utils::range(height)) {
    for (const Coord x : Utils::range(width)) {
      dst(pos.x + x, pos.y + y) = src(x, y);
    }
  }
}
