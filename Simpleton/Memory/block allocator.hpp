//
//  block allocator.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_block_allocator_hpp
#define engine_memory_block_allocator_hpp

#include <array>
#include <iostream>
#include "alloc.hpp"
#include <type_traits>

namespace Memory {
  enum class AllocFail {
    throw_bad_alloc,
    return_nullptr
  };

  template <typename Object_, size_t BLOCK_SIZE_, AllocFail FAIL_ = AllocFail::throw_bad_alloc>
  class BlockAllocator {
  public:
    using Object = Object_;
    static constexpr size_t BLOCK_SIZE = BLOCK_SIZE_;
    static constexpr AllocFail FAIL = FAIL_;
    
    static_assert(std::is_default_constructible_v<Object>, "Object must be default constructible");
    static_assert(std::is_destructible_v<Object>, "Object must be destructible");
    static_assert(BLOCK_SIZE != 0, "Block size must be greater than 0");
  
  private:
    union Block {
      Block *nextFree;
      std::array<Object, BLOCK_SIZE> objects;
    };
  
  public:
    explicit BlockAllocator(const size_t numBlocks)
      : blocks{allocArr<Block>(numBlocks)},
        numBlocks{numBlocks},
        head{blocks},
        allocations{0} {
      if (numBlocks != 0) {
        Block *prevBlock = blocks;
        Block *const lastBlock = blocks + (numBlocks - 1);
        while (prevBlock != lastBlock) {
          Block *const nextBlock = prevBlock + 1;
          prevBlock->nextFree = nextBlock;
          prevBlock = nextBlock;
        }
      }
    }
    ~BlockAllocator() {
      free(blocks);
      if (allocations != 0) {
        if (allocations == 1) {
          std::cout << "1 block was ";
        } else {
          std::cout << allocations << " blocks were ";
        }
        std::cout << "not freed before the allocator was destroyed\n";
      }
    }
    
    BlockAllocator(BlockAllocator &&) = default;
    BlockAllocator &operator=(BlockAllocator &&) = default;
    
    /// Are there any free blocks available for allocation?
    bool canAlloc() const {
      return head;
    }
    /// How many blocks have been allocated?
    size_t allocCount() const {
      return allocations;
    }
    /// How many blocks have not been allocated?
    size_t freeCount() const {
      return numBlocks - allocations;
    }
    
  private:
    template <typename T>
    struct SmartRefHelper {
      using type = std::conditional_t<
        std::is_trivially_copyable_v<T> && sizeof(T) <= 2 * sizeof(void *),
        const T,
        const T &
      >;
    };
    template <typename T>
    using SmartRef = typename SmartRefHelper<T>::type;
   
  public:
    /// Allocate a block. Throws std::bad_alloc if there are no free blocks
    template <typename... Args>
    Object *alloc(Args &&... args) {
      if (head == nullptr) {
        if constexpr (FAIL == AllocFail::throw_bad_alloc) {
          throw std::bad_alloc();
        } else if constexpr (FAIL == AllocFail::return_nullptr) {
          return nullptr;
        }
      }
      ++allocations;
      Block *const newBlock = head;
      head = head->nextFree;
      if constexpr (BLOCK_SIZE == 1) {
        moveConstruct(newBlock, std::forward<Args>(args)...);
      } else {
        copyConstruct<SmartRef<Args>...>(newBlock, args...);
      }
      return newBlock->objects.data();
    }
    
    /// Deallocate a block
    void free(Object *const object) {
      if (object == nullptr) {
        return;
      }
      rangeCheck(object);
      Block *const block = reinterpret_cast<Block *>(object);
      destroy(block);
      block->nextFree = head;
      head = block;
      --allocations;
    }
  
  private:
    Block *blocks;
    size_t numBlocks;
    Block *head;
    size_t allocations;
    
    template <typename... Args>
    void copyConstruct(Block *const block, Args... args) {
      if constexpr (sizeof...(Args) == 0 && std::is_trivially_default_constructible_v<Object>) {
        return;
      }
      for (Object &o : block->objects) {
        new (&o) Object{args...};
      }
    }
    template <typename... Args>
    void moveConstruct(Block *const block, Args &&... args) {
      if constexpr (sizeof...(Args) == 0 && std::is_trivially_default_constructible_v<Object>) {
        return;
      }
      static_assert(BLOCK_SIZE == 1);
      new (block->objects.data()) Object{std::forward<Args>(args)...};
    }
    
    void destroy(Block *const block) {
      if constexpr (std::is_trivially_destructible_v<Object>) {
        return;
      }
      for (Object &o : block->objects) {
        o.~Object();
      }
    }
    
    void rangeCheck(const Object *const object) {
      const uintptr_t objectInt = reinterpret_cast<uintptr_t>(object);
      const uintptr_t firstInt = reinterpret_cast<uintptr_t>(blocks);
      const uintptr_t lastInt = reinterpret_cast<uintptr_t>(blocks + BLOCK_SIZE);
      if (objectInt < firstInt || objectInt >= lastInt) {
        throw std::range_error("Pointer to free was not allocated");
      }
      if ((objectInt - firstInt) % sizeof(Block) != 0) {
        throw std::range_error("Pointer to free was not aligned");
      }
    }
  };
}

#endif
