//
//  buffer.cpp
//  Game Engine
//
//  Created by Indi Kernick on 25/09/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "buffer.hpp"

size_t std::hash<Memory::Buffer>::operator()(const Memory::Buffer &buf) const {
  std::hash<std::shared_ptr<Byte>> hasher;
  return hasher(buf.mData);
}

Byte *Memory::alloc(size_t size) {
  return reinterpret_cast<Byte *>(operator new(size));
}

void Memory::free(Byte *data) {
  operator delete(data);
}

void Memory::nofree(Byte *) {}

Memory::Buffer::Buffer(size_t size)
  : mData(alloc(size), &free), mSize(size) {
  assert(mData);
  //size is allowed to be zero
}

Memory::Buffer::Buffer(size_t size, Zero)
  : mData(alloc(size), &free), mSize(size) {
  assert(mData);
  std::memset(begin(), 0, mSize);
}

Memory::Buffer::Buffer(size_t size, One)
  : mData(alloc(size), &free), mSize(size) {
  assert(mData);
  std::memset(begin(), 255, mSize);
}

Memory::Buffer::Buffer(size_t size, Byte byte)
  : mData(alloc(size), &free), mSize(size) {
  assert(mData);
  std::memset(begin(), byte, mSize);
}

Memory::Buffer::Buffer(void *data, size_t size, const std::function<void (Byte *)> &deleter)
  : mData(reinterpret_cast<Byte *>(data), deleter), mSize(size) {
  assert(mData);
}

bool Memory::Buffer::operator==(const Buffer &other) const {
  return mData == other.mData || //refer to same memory
         (
           mSize == other.mSize &&
           std::memcmp(data(), other.data(), mSize) == 0
         );
}

bool Memory::Buffer::operator!=(const Buffer &other) const {
  return mData != other.mData ||
         mSize != other.mSize ||
         std::memcmp(data(), other.data(), mSize) != 0;
}

bool Memory::Buffer::operator<(const Buffer &other) const {
  return mSize < other.mSize ||
         (
           mData != other.mData &&
           std::lexicographical_compare(begin(), end(), other.begin(), other.end())
         );
}

bool Memory::Buffer::sameMemory(const Buffer &other) const {
  return mData == other.mData;
}

void Memory::Buffer::swap(Memory::Buffer &other) {
  std::swap(mData, other.mData);
  std::swap(mSize, other.mSize);
}
