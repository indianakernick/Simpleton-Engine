//
//  blockallocator.cpp
//  Game Engine
//
//  Created by Indi Kernick on 7/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "blockallocator.hpp"

#ifdef DEBUG

const uint32_t Memory::BlockAllocator::PATTERN = 0xDEADBEEF;
Byte Memory::BlockAllocator::REPEATED_PATTERN[PADDING_SIZE];
bool Memory::BlockAllocator::repeatedPatternFilled = false;

Memory::BlockAllocator::BlockAllocator(size_t blocksNum, size_t itemSize, size_t itemsNum)
  : ITEM_SIZE(itemSize),
    ITEMS_NUM(itemsNum),
    MAX_ALLOC_SIZE(Math::max(ITEM_SIZE * ITEMS_NUM, MIN_ALLOC_SIZE)),
    BLOCK_SIZE(MAX_ALLOC_SIZE + PADDING_SIZE * 2),
    BLOCKS_NUM(blocksNum),
    freeBlocksNum(BLOCKS_NUM),
    memory(BLOCK_SIZE * BLOCKS_NUM),
    head(memory.begin() + PADDING_SIZE),
    arraySizes(BLOCKS_NUM, ZERO_INIT) {
  if (ITEM_SIZE * ITEMS_NUM < MIN_ALLOC_SIZE) {
    LOG_ERROR(MEMORY,
      "Alloc size (%lu) is too small. Using minimum (%llu) instead",
      ITEM_SIZE * ITEMS_NUM, MIN_ALLOC_SIZE);
  }
  assert(BLOCKS_NUM != 0 && "Must allocate at least one block");
  assert(ITEMS_NUM != 0 && "Must allocate at least one item per block");
  
  if (!repeatedPatternFilled) {
    memset_pattern4(REPEATED_PATTERN, &PATTERN, PADDING_SIZE);
    repeatedPatternFilled = true;
  }
  
  for (Byte *i = memory.begin() + PADDING_SIZE; i != memory.end(); i+=BLOCK_SIZE) {
    std::memcpy(i - PADDING_SIZE, REPEATED_PATTERN, PADDING_SIZE);
    assign(i, i + BLOCK_SIZE);
    std::memcpy(i + MAX_ALLOC_SIZE, REPEATED_PATTERN, PADDING_SIZE);
  }
  assign(memory.end() - PADDING_SIZE - MAX_ALLOC_SIZE, nullptr);
}


Memory::BlockAllocator::~BlockAllocator() {
  //if the allocator is destroyed before the blocks are freed then
  //the user will have pointers to deallocated memory
  if (freeBlocksNum != BLOCKS_NUM) {
    LOG_ERROR(MEMORY, "Allocator was destroyed before all blocks were freed");
  }
}

void Memory::BlockAllocator::dumpMemory(std::ostream &stream, bool format) {
  if (format) {
    for (Byte *i = memory.begin(); i != memory.end();) {
      i += PADDING_SIZE;
      size_t index = blockIndex(i),
             arraySize = sizeOfArray(i),
             allocSize = sizeOfAlloc(i),
             restSize = sizeOfRest(i);
      i -= PADDING_SIZE;
      
      stream << "  Block Number     " << index << '\n';
      stream << "  Array Size       " << arraySize << '\n';
      stream << "  Allocation size  " << allocSize << '\n';
      stream << "  Rest size        " << restSize << '\n';
      stream << "Before padding\n";
      
      formatMemory(stream, i, PADDING_SIZE);
      i += PADDING_SIZE;
      
      stream << "\n"
                "End before padding\n"
                "Begin Raw data\n";
      
      stream.write(reinterpret_cast<char *>(i), allocSize);
      i += allocSize;
      
      stream << "\n"
                "End Raw data\n"
                "Begin rest\n";
      
      formatMemory(stream, i, restSize);
      i += restSize;
      
      stream << "\n"
                "End rest\n"
                "After padding\n";
      
      formatMemory(stream, i, PADDING_SIZE);
      i += PADDING_SIZE;
      
      stream << "\n"
                "End after padding\n"
                "\n";
      
    }
  } else {
    stream.write(memory.cdata<char>(), memory.size());
  }
}

void *Memory::BlockAllocator::alloc() {
  if (!head) {
    throw OutOfMemory("Out of memory");
  }
  freeBlocksNum--;
  Byte *out = head;
  head = deref<Byte *>(head);
  setSizeOfArray(out, 1);
  if (ITEMS_NUM > 1 || ITEM_SIZE < MIN_ALLOC_SIZE) {
    memset_pattern4(out + ITEM_SIZE, &PATTERN, MAX_ALLOC_SIZE - ITEM_SIZE);
  }
  
  return out;
}

void Memory::BlockAllocator::free(void *ptr) {
  //this function is kind of funny becuase it will succeed to free an array
  //if you don't check for that
  assert(!isArray(ptr) && "Freeing array with the scalar version of free");
  freeArray(ptr);
}

void *Memory::BlockAllocator::allocArray(size_t size) {
  if (size > ITEMS_NUM) {
    throw std::range_error("Cannot allocate array greater than maximum size");
  }
  void *out = alloc();
  setSizeOfArray(out, size);
  return out;
}

void Memory::BlockAllocator::freeArray(void *ptr) {
  if (!isWithinPool(ptr)) {
    throw BadPtr("Pointer out of range");
  }
  if (!isAligned(ptr)) {
    throw BadPtr("Pointer not aligned with BLOCK_SIZE");
  }
  
  assert(isAlloc(ptr) && "Freeing memory that was not allocated");
  
  const bool before = beforePaddingModified(ptr);
  const bool after = afterPaddingModified(ptr);
  
  if (before) {
    throw UnderRun("Padding before allocation was modified");
  }
  if (after) {
    throw OverRun("Padding after an allocation was modified");
  }
  if (restModified(ptr)) {
    throw OverRun("Memory outside of allocated part of block was modified");
  }
  
  freeBlocksNum++;
  *reinterpret_cast<Byte **>(ptr) = head;
  head = toByte(ptr);
}

bool Memory::BlockAllocator::isAligned(void *ptr) const {
  return memory.index(ptr) % BLOCK_SIZE == PADDING_SIZE;
}

size_t Memory::BlockAllocator::sizeOfArray(void *ptr) const {
  return arraySizes[blockIndex(ptr)];
}

size_t Memory::BlockAllocator::blockIndex(void *ptr) const {
  if (!isValid(ptr)) {
    throw BadPtr("Invalid pointer");
  }
  return memory.index(ptr) / BLOCK_SIZE;
}

bool Memory::BlockAllocator::beforePaddingModified(void *ptr) {
  if (std::memcmp(toByte(ptr) - PADDING_SIZE, REPEATED_PATTERN, PADDING_SIZE)) {
    std::memcpy(toByte(ptr) - PADDING_SIZE, REPEATED_PATTERN, PADDING_SIZE);
    return true;
  } else {
    return false;
  }
}

bool Memory::BlockAllocator::afterPaddingModified(void *ptr) {
  if (std::memcmp(toByte(ptr) + MAX_ALLOC_SIZE, REPEATED_PATTERN, PADDING_SIZE)) {
    std::memcpy(toByte(ptr) + MAX_ALLOC_SIZE, REPEATED_PATTERN, PADDING_SIZE);
    return true;
  } else {
    return false;
  }
}

bool Memory::BlockAllocator::paddingModified(void *ptr) {
  //have to call both of them no matter what
  bool modifed = false;
  modifed |= beforePaddingModified(ptr);
  modifed |= afterPaddingModified(ptr);
  return modifed;
}

bool Memory::BlockAllocator::restModified(void *ptr) {
  const uint8_t *splitPattern = reinterpret_cast<const uint8_t *>(&PATTERN);
  Byte *start = toByte(ptr) + sizeOfAlloc(ptr);
  const size_t end = sizeOfRest(ptr);
  for (size_t i = 0; i != end; i++) {
    if (start[i] != splitPattern[i % 4]) {
      return true;
    }
  }
  return false;
}

void Memory::BlockAllocator::setSizeOfArray(void *ptr, size_t newSize) {
  arraySizes[blockIndex(ptr)] = newSize;
}

void Memory::BlockAllocator::formatMemory(std::ostream &stream, Byte *ptr, size_t size) {
  //the memory has to read in 4 byte chunks becuase most machines are little
  //endian so i read it in 1 byte chunks it won't spell out DEADBEEF
  //4 byte chunks  DEADBEEF
  //2 byte chunks  BEEFDEAD
  //1 byte chunks  EFBEADDE
  ptrdiff_t trailingBytes = size % 4;
  uint32_t *end = reinterpret_cast<uint32_t *>(ptr + size - trailingBytes);
  for (uint32_t *i = reinterpret_cast<uint32_t *>(ptr); i != end; i++) {
    stream << Math::hexU32(*i);
  }
  if (trailingBytes) {
    uint32_t trail;
    std::memcpy(&trail, end, trailingBytes);
    stream << Math::hexU32(trail);
  }
}

#else // RELEASE

Memory::BlockAllocator::BlockAllocator(size_t blocksNum, size_t itemSize, size_t itemsNum)
  : ITEM_SIZE(itemSize),
    ITEMS_NUM(itemsNum),
    MAX_ALLOC_SIZE(Math::max(ITEM_SIZE * ITEMS_NUM, MIN_ALLOC_SIZE)),
    BLOCK_SIZE(MAX_ALLOC_SIZE),
    BLOCKS_NUM(blocksNum),
    freeBlocksNum(BLOCKS_NUM),
    memory(BLOCK_SIZE * BLOCKS_NUM),
    head(memory.begin()),
    arraySizes(BLOCKS_NUM, ZERO_INIT) {
  Byte *last = memory.end() - MAX_ALLOC_SIZE;
  for (Byte *i = memory.begin(); i != last; i+=BLOCK_SIZE) {
    assign(i, i + BLOCK_SIZE);
  }
  assign(last, nullptr);
}

bool Memory::BlockAllocator::isAligned(void *ptr) const {
  return memory.index(ptr) % BLOCK_SIZE == 0;
}

#endif

bool Memory::BlockAllocator::isValid(void *ptr) const {
  return isAligned(ptr) && isWithinPool(ptr);
}

bool Memory::BlockAllocator::isWithinPool(void *ptr) const {
  return ptr >= memory.begin() && ptr < memory.end();
}

bool Memory::BlockAllocator::isArray(void *ptr) const {
  return sizeOfArray(ptr) > 1;
}

bool Memory::BlockAllocator::isAlloc(void *ptr) const {
  return sizeOfArray(ptr);
}

size_t Memory::BlockAllocator::sizeOfAlloc(void *ptr) const {
  return sizeOfArray(ptr) * ITEM_SIZE;
}

size_t Memory::BlockAllocator::sizeOfRest(void *ptr) const {
  return MAX_ALLOC_SIZE - sizeOfAlloc(ptr);
}

size_t Memory::BlockAllocator::getBlocksNum() const {
  return BLOCKS_NUM;
}

size_t Memory::BlockAllocator::getItemSize() const {
  return ITEM_SIZE;
}

size_t Memory::BlockAllocator::getItemsNum() const {
  return ITEMS_NUM;
}

size_t Memory::BlockAllocator::getMaxAllocSize() const {
  return MAX_ALLOC_SIZE;
}

size_t Memory::BlockAllocator::getBlockSize() const {
  return BLOCK_SIZE;
}

size_t Memory::BlockAllocator::getFreeBlocks() const {
  return freeBlocksNum;
}
