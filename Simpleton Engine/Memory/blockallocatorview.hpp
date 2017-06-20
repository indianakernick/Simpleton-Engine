//
//  blockallocatorview.hpp
//  Game Engine
//
//  Created by Indi Kernick on 7/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_blockallocatorview_hpp
#define engine_memory_blockallocatorview_hpp

#include "blockallocator.hpp"
#include <new>

namespace Memory {
  template <typename T>
  class BlockAllocatorView {
  public:
    explicit BlockAllocatorView(const BlockAllocator &allocator)
      : allocator(allocator) {
      assert(allocator.getItemSize() == sizeof(T));
    }
    BlockAllocatorView(size_t blocksNum, size_t itemsNum = 1)
      : allocator(blocksNum, sizeof(T), itemsNum) {}
    
    template <typename ...ARGS>
    T *alloc(ARGS ...args) {
      return new (allocator.alloc()) T(std::forward<ARGS>(args)...);
    }
    void free(T *ptr) {
      //call destructor
      ptr->~T();
      allocator.free(ptr);
    }
    template <typename ...ARGS>
    T *allocArray(size_t size, ARGS ...args) {
      T *ptr = reinterpret_cast<T *>(allocator.allocArray(size));
      if (std::is_scalar<T>::value && sizeof...(ARGS) == 0) {
        return ptr;
      } else {
        for (T *i = ptr; i < ptr + size; i++) {
          new (i) T(std::forward<ARGS>(args)...);
        }
      }
    }
    void freeArray(T *ptr) {
      T *end = ptr + allocator.sizeOfArray(ptr);
      for (T *i = ptr; i < end; i++) {
        i->~T();
      }
      allocator.freeArray(ptr);
    }
  private:
    BlockAllocator allocator;
  };
}

#endif
