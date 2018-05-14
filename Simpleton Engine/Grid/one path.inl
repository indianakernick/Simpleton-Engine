//
//  one path.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 14/5/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include "../Math/dir.hpp"

template <typename Tile, typename Function>
std::vector<Grid::Pos> Grid::onePath(
  const Grid<Tile> &grid,
  Function &&notPath,
  const Pos start,
  const Pos end
) {
  if (grid.outOfRange(start) || grid.outOfRange(end)) {
    return {};
  }
  
  Pos pos = start;
  std::vector<Pos> path;
  path.push_back(start);
  Math::Dir prevDir = Math::Dir::NONE;
  
  do {
    bool deadEnd = true;
    
    for (const Math::Dir dir : Math::DIR_RANGE) {
      if (dir == Math::opposite(prevDir)) {
        continue;
      }
      
      const Pos neighPos = pos + Math::ToVec<Coord>::conv(dir);
      if (grid.outOfRange(neighPos) || notPath(grid[neighPos])) {
        continue;
      }
      
      path.push_back(neighPos);
      if (neighPos == end) {
        return path;
      }
      
      deadEnd = false;
      prevDir = dir;
      pos = neighPos;
      
      break;
    }
    
    if (deadEnd) {
      return {};
    }
  } while (true);
}
