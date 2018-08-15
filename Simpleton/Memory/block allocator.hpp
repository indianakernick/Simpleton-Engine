//
//  block allocator.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_block_allocator_hpp
#define engine_memory_block_allocator_hpp

#include <new>
#include <memory>
#include <iostream>
#include <type_traits>

namespace Memory {
  template <typename Object_, size_t BLOCK_SIZE_>
  class BlockAllocator {
  public:
    using Object = Object_;
    static constexpr size_t BLOCK_SIZE = BLOCK_SIZE_;
    
    static_assert(std::is_default_constructible<Object>::value, "Object must be default constructible");
    static_assert(std::is_destructible<Object>::value, "Object must be destructible");
    static_assert(BLOCK_SIZE != 0, "Block size must be greater than 0");
  
  private:
    union Block {
      Block *nextFree;
      Object objects[BLOCK_SIZE];
    };
  
  public:
    explicit BlockAllocator(const size_t numBlocks)
      : blocks(std::make_unique<Block []>(numBlocks)),
        numBlocks(numBlocks),
        head(blocks.get()) {
      if (numBlocks != 0) {
        Block *prevBlock = blocks.get();
        Block *const lastBlock = blocks.get() + (numBlocks - 1);
        while (prevBlock != lastBlock) {
          Block *const nextBlock = prevBlock + 1;
          prevBlock->nextFree = nextBlock;
          prevBlock = nextBlock;
        }
      }
    }
    ~BlockAllocator() {
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
    
    ///Returns whether there are any free blocks available for allocation
    bool canAlloc() const {
      return head;
    }
    
  private:
    template <typename T>
    struct SmartRefHelper {
      using type = std::conditional_t<
        std::is_trivially_copyable<T>::value && sizeof(T) <= 2 * sizeof(void *),
        const T,
        const T &
      >;
    };
    template <typename T>
    using SmartRef = typename SmartRefHelper<T>::type;
   
  public:
    ///Allocate a block. Throws std::bad_alloc if there are no free blocks
    template <typename ...Args>
    std::enable_if_t< // Have to make the condition dependant on this template
      BLOCK_SIZE != 1 && (!std::is_void<Args>::value && ...),
      Object *
    >
    alloc(const Args &... args) {
      if (head == nullptr) {
        throw std::bad_alloc();
      }
      ++allocations;
      Block *const newBlock = head;
      head = head->nextFree;
      copyConstruct<SmartRef<Args>...>(newBlock, args...);
      return newBlock->objects;
    }
    ///Allocate a block. Throws std::bad_alloc if there are no free blocks
    template <typename ...Args>
    std::enable_if_t<  // Have to make the condition dependant on this template
      BLOCK_SIZE == 1 && (!std::is_void<Args>::value && ...),
      Object *
    >
    alloc(Args &&... args) {
      if (head == nullptr) {
        throw std::bad_alloc();
      }
      ++allocations;
      Block *const newBlock = head;
      head = head->nextFree;
      moveConstruct(newBlock, std::forward<Args>(args)...);
      return newBlock->objects;
    }
    
    ///Deallocate a block
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
    std::unique_ptr<Block []> blocks;
    size_t numBlocks;
    Block *head;
    size_t allocations = 0;
    
    template <typename ...Args>
    void copyConstruct(Block *const block, Args... args) {
      if constexpr (sizeof...(Args) == 0 && std::is_trivially_default_constructible<Object>::value) {
        return;
      }
      Object *const endObject = block->objects + BLOCK_SIZE;
      for (Object *o = block->objects; o != endObject; ++o) {
        new (o) Object(args...);
      }
    }
    template <typename ...Args>
    void moveConstruct(Block *const block, Args &&... args) {
      if constexpr (sizeof...(Args) == 0 && std::is_trivially_default_constructible<Object>::value) {
        return;
      }
      static_assert(BLOCK_SIZE == 1);
      new (block->objects) Object(std::forward<Args>(args)...);
    }
    
    void destroy(Block *const block) {
      if constexpr (std::is_trivially_destructible<Object>::value) {
        return;
      }
      Object *const endObject = block->objects + BLOCK_SIZE;
      for (Object *o = block->objects; o != endObject; ++o) {
        o->~Object();
      }
    }
    
    void rangeCheck(const Object *const object) {
      const uintptr_t objectInt = reinterpret_cast<uintptr_t>(object);
      const uintptr_t firstInt = reinterpret_cast<uintptr_t>(blocks.get());
      const uintptr_t lastInt = reinterpret_cast<uintptr_t>(blocks.get() + BLOCK_SIZE);
      if (objectInt < firstInt || objectInt > lastInt) {
        throw std::range_error("Pointer to free was not allocated");
      }
      if ((objectInt - firstInt) % sizeof(Block) != 0) {
        throw std::range_error("Pointer to free was not aligned");
      }
    }
  };
}

#endif
