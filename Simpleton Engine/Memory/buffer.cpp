//
//  buffer.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 25/09/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "buffer.hpp"

#include "../Utils/null check.hpp"

namespace Memory {
  Byte *alloc(const size_t size) {
    return reinterpret_cast<Byte *>(operator new(size));
  }

  void free(void *const data) {
    operator delete(data);
  }
  
  void noFree(void *const) {}
}

Memory::Buffer::Buffer(const size_t size)
  : mData(alloc(size), &free), mSize(size) {}

Memory::Buffer::Buffer(const size_t size, Zero)
  : mData(alloc(size), &free), mSize(size) {
  std::memset(data(), 0, mSize);
}

Memory::Buffer::Buffer(const size_t size, One)
  : mData(alloc(size), &free), mSize(size) {
  std::memset(data(), 255, mSize);
}

Memory::Buffer::Buffer(const size_t size, const Byte byte)
  : mData(alloc(size), &free), mSize(size) {
  std::memset(data(), byte, mSize);
}

Memory::Buffer::Buffer(void *const data, const size_t size)
  : mData(reinterpret_cast<Byte *>(data), &free), mSize(size) {
  Utils::throwIfNull(data);
}

Memory::Buffer::Buffer(void *const data, const size_t size, NoDelete)
  : mData(reinterpret_cast<Byte *>(data), &noFree), mSize(size) {
  Utils::throwIfNull(data);
}

bool Memory::Buffer::operator==(const Buffer &other) const {
  return mSize == other.mSize && std::memcmp(data(), other.data(), mSize) == 0;
}

bool Memory::Buffer::operator!=(const Buffer &other) const {
  return mSize != other.mSize || std::memcmp(data(), other.data(), mSize) != 0;
}

bool Memory::Buffer::operator<(const Buffer &other) const {
  return mSize < other.mSize || std::memcmp(data(), other.data(), mSize) < 0;
}

void Memory::Buffer::swap(Buffer &other) {
  std::swap(mData, other.mData);
  std::swap(mSize, other.mSize);
}

void Memory::Buffer::copyFrom(const Buffer &buffer) {
  std::memcpy(data(), buffer.data(), mSize < buffer.mSize ? mSize : buffer.mSize);
}
