//
//  zsort.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 21/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include <algorithm>

inline void G2D::sort(Quad *const begin, Quad *const end) {
  std::sort(begin, end, [] (const Quad &a, const Quad &b) -> bool {
    return a[0].pos.z > b[0].pos.z;
  });
}

inline void G2D::sortCenter(Quad *const begin, Quad *const end) {
  std::sort(begin, end, [] (const Quad a, const Quad b) -> bool {
    const auto aDepth = a[0].pos.z + a[1].pos.z + a[2].pos.z + a[3].pos.z;
    const auto bDepth = b[0].pos.z + b[1].pos.z + b[2].pos.z + b[3].pos.z;
    return aDepth > bDepth;
  });
}

inline void G2D::sortDeep(Quad *const begin, Quad *const end) {
  std::sort(begin, end, [] (const Quad a, const Quad b) -> bool {
    const auto aDepth = std::max({a[0].pos.z, a[1].pos.z, a[2].pos.z, a[3].pos.z});
    const auto bDepth = std::max({b[0].pos.z, b[1].pos.z, b[2].pos.z, b[3].pos.z});
    return aDepth > bDepth;
  });
}

inline void G2D::sortShallow(Quad *const begin, Quad *const end) {
  std::sort(begin, end, [] (const Quad a, const Quad b) -> bool {
    const auto aDepth = std::min({a[0].pos.z, a[1].pos.z, a[2].pos.z, a[3].pos.z});
    const auto bDepth = std::min({b[0].pos.z, b[1].pos.z, b[2].pos.z, b[3].pos.z});
    return aDepth > bDepth;
  });
}
