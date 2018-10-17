//
//  buffer.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 25/09/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include <utility>

inline Memory::Buffer::Buffer(const size_t size) noexcept
  : mData{allocBytes(size)}, mSize{size} {}

inline Memory::Buffer::Buffer(const size_t size, Zero) noexcept
  : Buffer{size, std::byte{}} {}

inline Memory::Buffer::Buffer(const size_t size, One) noexcept
  : Buffer{size, ~std::byte{}} {}

inline Memory::Buffer::Buffer(const size_t size, const std::byte byte) noexcept
  : Buffer{size} {
  std::memset(mData, static_cast<int>(byte), mSize);
}

inline Memory::Buffer::Buffer(Buffer &&other) noexcept
  : mData{std::exchange(other.mData, nullptr)},
    mSize{std::exchange(other.mSize, 0)} {}

inline Memory::Buffer &Memory::Buffer::operator=(Buffer &&other) noexcept {
  dealloc(mData);
  mData = std::exchange(other.mData, nullptr);
  mSize = std::exchange(other.mSize, 0);
  return *this;
}

inline Memory::Buffer::~Buffer() noexcept {
  dealloc(mData);
}

inline bool Memory::Buffer::operator==(const Buffer &other) const noexcept {
  return mSize == other.mSize && std::memcmp(mData, other.mData, mSize) == 0;
}

inline bool Memory::Buffer::operator!=(const Buffer &other) const noexcept {
  return !(*this == other);
}

inline bool Memory::Buffer::operator<(const Buffer &other) const noexcept {
  if (mSize < other.mSize) {
    return true;
  } else if (other.mSize < mSize) {
    return false;
  } else {
    return std::memcmp(mData, other.mData, mSize) < 0;
  }
}

inline bool Memory::Buffer::operator>(const Buffer &other) const noexcept {
  return other < *this;
}

inline bool Memory::Buffer::operator<=(const Buffer &other) const noexcept {
  return !(other < *this);
}

inline bool Memory::Buffer::operator>=(const Buffer &other) const noexcept {
  return !(*this < other);
}

inline void Memory::Buffer::swap(Buffer &other) noexcept {
  std::swap(mData, other.mData);
  std::swap(mSize, other.mSize);
}

inline void Memory::Buffer::copyFrom(const Buffer &buffer) noexcept {
  std::memcpy(mData, buffer.mData, mSize < buffer.mSize ? mSize : buffer.mSize);
}

inline Memory::Buffer Memory::Buffer::dup() const noexcept {
  return dup(mSize);
}

inline Memory::Buffer Memory::Buffer::dup(const size_t size) const noexcept {
  Buffer buf{size};
  buf.copyFrom(*this);
  return buf;
}

inline Memory::Buffer Memory::Buffer::dup(const size_t size, const std::byte byte) const noexcept {
  Buffer buf = dup(size);
  if (size > mSize) {
    std::memset(buf.mData, static_cast<int>(byte), size - mSize);
  }
  return buf;
}

inline std::byte *Memory::Buffer::findPtr(const std::byte byte) noexcept {
  return reinterpret_cast<std::byte *>(
    std::memchr(mData, static_cast<int>(byte), mSize)
  );
}

inline const std::byte *Memory::Buffer::findPtr(const std::byte byte) const noexcept {
  return reinterpret_cast<const std::byte *>(
    std::memchr(mData, static_cast<int>(byte), mSize)
  );
}

inline size_t Memory::Buffer::findIdx(const std::byte byte) const noexcept {
  const std::byte *const ptr = findPtr(byte);
  if (ptr == nullptr) {
    return mSize;
  } else {
    return ptr - mData;
  }
}

inline void Memory::swap(Buffer &a, Buffer &b) {
  a.swap(b);
}
