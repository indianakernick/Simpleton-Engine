//
//  null check.hpp
//  Game Engine
//
//  Created by Indi Kernick on 30/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_null_check_hpp
#define engine_utils_null_check_hpp

namespace Utils {
  template <typename Type>
  const Type *zeroIfNull(const Type * const ptr) {
    static const Type ZERO = {};
    return ptr == nullptr ? &ZERO : ptr;
  }

  //C++ doesn't have function aliases
  //Am I the only person who thinks they are useful?

  template <typename Char>
  const Char *emptyIfNull(const Char * const ptr) {
    static const Char ZERO = {};
    return ptr == nullptr ? &ZERO : ptr;
  }
}

#endif
