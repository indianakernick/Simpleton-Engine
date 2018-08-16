//
//  buffer.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 25/09/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "../Utils/null check.hpp"

inline Memory::Buffer::Buffer(const size_t size)
  : mData(allocBytes(size), &free), mSize(size) {}

inline Memory::Buffer::Buffer(const size_t size, Zero)
  : mData(allocBytes(size), &free), mSize(size) {
  std::memset(data(), 0, mSize);
}

inline Memory::Buffer::Buffer(const size_t size, One)
  : mData(allocBytes(size), &free), mSize(size) {
  std::memset(data(), 255, mSize);
}

inline Memory::Buffer::Buffer(const size_t size, const Byte byte)
  : mData(allocBytes(size), &free), mSize(size) {
  std::memset(data(), byte, mSize);
}

inline Memory::Buffer::Buffer(void *const data, const size_t size)
  : mData(reinterpret_cast<Byte *>(data), &free), mSize(size) {
  Utils::throwIfNull(data);
}

inline Memory::Buffer::Buffer(void *const data, const size_t size, NoDelete)
  : mData(reinterpret_cast<Byte *>(data), &noFree), mSize(size) {
  Utils::throwIfNull(data);
}

inline bool Memory::Buffer::operator==(const Buffer &other) const {
  return mSize == other.mSize && std::memcmp(data(), other.data(), mSize) == 0;
}

inline bool Memory::Buffer::operator!=(const Buffer &other) const {
  return mSize != other.mSize || std::memcmp(data(), other.data(), mSize) != 0;
}

inline bool Memory::Buffer::operator<(const Buffer &other) const {
  if (mSize < other.mSize) {
    return true;
  } else if (other.mSize < mSize) {
    return false;
  } else {
    return std::memcmp(data(), other.data(), mSize) < 0;
  }
}

inline void Memory::Buffer::swap(Buffer &other) {
  std::swap(mData, other.mData);
  std::swap(mSize, other.mSize);
}

inline void Memory::Buffer::copyFrom(const Buffer &buffer) {
  std::memcpy(data(), buffer.data(), mSize < buffer.mSize ? mSize : buffer.mSize);
}

inline void Memory::Buffer::resize(const size_t size) {
  mData = std::unique_ptr<Byte, void(*)(void *)>(
    allocBytes(size), mData.get_deleter()
  );
  mSize = size;
}

inline void Memory::Buffer::resizeCopy(const size_t size) {
  auto temp = std::unique_ptr<Byte, void(*)(void *)>(
    allocBytes(size), mData.get_deleter()
  );
  std::memcpy(temp.get(), mData.get(), mSize < size ? mSize : size);
  mData = std::move(temp);
  mSize = size;
}
