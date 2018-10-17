//
//  alloc.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 16/8/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_alloc_hpp
#define engine_memory_alloc_hpp

#include <new>
#include <cstring>
#include <cstddef>

namespace Memory {
  /// Set std::terminate as the new_handler. Since Memory::alloc is noexcept,
  /// the default new_handler will throw std::bad_alloc and std::terminate will
  /// be called. This new handler calls std::terminate directly.
  inline void setNewHandler() {
    std::set_new_handler(std::terminate);
  }

  /// Allocate memory. Allocation failure results in std::terminate
  template <typename Type = void>
  Type *alloc(const size_t bytes, const size_t align = alignof(std::max_align_t)) noexcept {
    return static_cast<Type *>(operator new(
      bytes, std::align_val_t{align}
    ));
  }

  /// Allocate uninitialized bytes
  inline std::byte *allocBytes(const size_t bytes) noexcept {
    return alloc<std::byte>(bytes);
  }
  
  /// Allocate uninitialized object
  template <typename Object>
  Object *allocObj() noexcept {
    return alloc<Object>(sizeof(Object), alignof(Object));
  }
  
  /// Allocate uninitialized array of objects
  template <typename Object>
  Object *allocArr(const size_t count) noexcept {
    return alloc<Object>(count * sizeof(Object), alignof(Object));
  }

  /// Deallocate memory
  inline void dealloc(void *const ptr) noexcept {
    operator delete(ptr);
  }
  
  /// Reallocate memory
  template <typename Type = void>
  Type *realloc(Type *const ptr, const size_t oldSize, const size_t newSize) noexcept {
    Type *const newPtr = alloc<Type>(newSize);
    std::memcpy(newPtr, ptr, oldSize < newSize ? oldSize : newSize);
    dealloc(ptr);
    return newPtr;
  }
}

#endif
