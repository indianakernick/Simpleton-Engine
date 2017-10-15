//
//  block allocator.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef block_allocator_hpp
#define block_allocator_hpp

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
    
    static_assert(std::is_default_constructible<Object_>::value, "Object must be default constructible");
    static_assert(std::is_destructible<Object_>::value, "Object must be default constructible");
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
      if (allocated) {
        std::cout << "At least one block was not freed before the allocator was destroyed\n";
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
        std::is_trivially_copyable<T>::value && sizeof(T) <= sizeof(size_t) * 4,
        const T,
        const T &
      >;
    };
    template <typename T>
    using SmartRef = typename SmartRefHelper<T>::type;
   
  public:
    ///Allocate a block. Throws std::bad_alloc if there are no free blocks
    template <typename ...Args>
    Object *alloc(const Args &... args) {
      if (head == nullptr) {
        throw std::bad_alloc();
      }
      allocated = true;
      Block *const newBlock = head;
      head = head->nextFree;
      if constexpr (sizeof...(Args) == 0) {
        defaultConstruct(newBlock);
      } else {
        construct<SmartRef<Args>...>(newBlock, args...);
      }
      return newBlock;
    }
    
    ///Deallocate a block
    void free(Object *const object) {
      if (object == nullptr) {
        return;
      }
      rangeCheck(object);
      destroy(object);
      Block *const block = reinterpret_cast<Block *>(object);
      block->nextFree = head;
      head = block;
    }
  
  private:
    std::unique_ptr<Block []> blocks;
    size_t numBlocks;
    Block *head;
    //at least one block is allocated
    bool allocated = false;
    
    void defaultConstruct(Block *const block) {
      if constexpr (std::is_trivially_default_constructible<Object>::value) {
        return;
      }
      Object *const endObject = block->objects + BLOCK_SIZE;
      for (Object *o = block->objects; o != endObject; ++o) {
        new (o) Object();
      }
    }
    
    template <typename ...Args>
    void construct(Block *const block, Args... args) {
      Object *const endObject = block->objects + BLOCK_SIZE;
      for (Object *o = block->objects; o != endObject; ++o) {
        new (o) Object(args...);
      }
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
