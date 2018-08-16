//
//  alloc.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 16/8/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_alloc_hpp
#define engine_memory_alloc_hpp

#include "byteptr.hpp"

namespace Memory {
  /// Allocate uninitialized bytes
  inline Byte *allocBytes(const size_t bytes) {
    return static_cast<Byte *>(operator new(bytes));
  }
  
  /// Allocate uninitialized object
  template <typename Object>
  Object *allocObj() {
    return static_cast<Object *>(operator new(
      sizeof(Object), std::align_val_t{alignof(Object)}
    ));
  }
  
  /// Allocate uninitialized array of objects
  template <typename Object>
  Object *allocArr(const size_t count) {
    return static_cast<Object *>(operator new(
      sizeof(Object) * count, std::align_val_t{alignof(Object)}
    ));
  }

  /// Free memory
  inline void free(void *const ptr) {
    operator delete(ptr);
  }
  
  /// Don't free memory
  inline void noFree(void *const) {}
}

#endif
