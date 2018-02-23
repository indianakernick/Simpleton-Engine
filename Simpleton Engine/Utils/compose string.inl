//
//  compose string.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include "endian.hpp"

inline Utils::ComposeString::ComposeString(const size_t capacity)
  : string(std::make_unique<char []>(capacity)), size(0), capacity(capacity) {}

inline char *Utils::ComposeString::begin() {
  return string.get();
}

inline char *Utils::ComposeString::curr() {
  return string.get() + size;
}

inline char *Utils::ComposeString::end() {
  return string.get() + capacity;
}

inline std::string_view Utils::ComposeString::view() const {
  return {string.get(), size};
}

inline size_t Utils::ComposeString::freeSpace() const {
  return capacity - size;
}

inline void Utils::ComposeString::reserve(const size_t newCap) {
  if (newCap > capacity) {
    setCapacity(newCap);
  }
}

inline void Utils::ComposeString::reserveToFit(const size_t extra) {
  if (size + extra > capacity) {
    setCapacity((capacity + extra) * 2);
  }
}

inline void Utils::ComposeString::addSize(const size_t extra) {
  size += extra;
}

inline bool Utils::ComposeString::empty() const {
  return size == 0;
}

inline void Utils::ComposeString::write(const char c) {
  if (size == capacity) {
    setCapacity((capacity + 1) * 2);
  }
  *curr() = c;
  ++size;
}

inline void Utils::ComposeString::write(const char *const d, const size_t s) {
  reserveToFit(s);
  std::memcpy(curr(), d, s);
  size += s;
}

inline void Utils::ComposeString::write(const std::string_view view) {
  write(view.data(), view.size());
}

template <typename Number>
void Utils::ComposeString::writeNumber(const Number num) {
  write(std::to_string(num));
  /*
  @TODO
  
  reserveToFit(64);
  const auto [ptr, ec] = std::to_chars(curr(), end(), num);
  if (!ec) {
    size = ptr - string.get();
  }
  */
}

template <typename Number>
void Utils::ComposeString::writeNumberLil(const Number num) {
  reserveToFit(sizeof(Number));
  write(reinterpret_cast<const char *>(&toLilEndian(num)), sizeof(Number));
}

template <typename Number>
void Utils::ComposeString::writeNumberBig(const Number num) {
  reserveToFit(sizeof(Number));
  write(reinterpret_cast<const char *>(&toBigEndian(num)), sizeof(Number));
}

template <typename Number>
void Utils::ComposeString::writeNumbersLil(const Number *n, const size_t s) {
  reserveToFit(sizeof(size_t) + sizeof(Number) * s);
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
  std::memcpy(curr(), &s, sizeof(size_t));
  std::memcpy(curr() + sizeof(size_t), n, sizeof(Number) * s);
  addSize(sizeof(size_t) + sizeof(Number) * s);
#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
  const auto swappedSize = endianSwap(s);
  std::memcpy(curr(), &swappedSize, sizeof(size_t));
  size += sizeof(size_t);
  const Number *const end = n + s;
  for (; n != end; ++n) {
    const auto swapped = endianSwap(*n);
    std::memcpy(curr(), &swapped, sizeof(Number));
    size += sizeof(Number);
  }
#else
#error Unknown endianess
#endif
}

template <typename Number>
void Utils::ComposeString::writeNumbersBig(const Number *n, const size_t s) {
  reserveToFit(sizeof(size_t) + sizeof(Number) * s);
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
  const auto swappedSize = endianSwap(s);
  std::memcpy(curr(), &swappedSize, sizeof(size_t));
  size += sizeof(size_t);
  const Number *const end = n + s;
  for (; n != end; ++n) {
    const auto swapped = endianSwap(*n);
    std::memcpy(curr(), &swapped, sizeof(Number));
    size += sizeof(Number);
  }
#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
  std::memcpy(curr(), &s, sizeof(size_t));
  std::memcpy(curr() + sizeof(size_t), n, sizeof(Number) * s);
  addSize(sizeof(size_t) + sizeof(Number) * s);
#else
#error Unknown endianess
#endif
}
        
template <typename Enum>
void Utils::ComposeString::writeEnum(const Enum e, const std::string_view *names) {
  write(names[static_cast<size_t>(e)]);
}

inline void Utils::ComposeString::open(const std::string &opening, const std::string &closing) {
  write(opening);
  closingStrings.push(closing);
}

inline void Utils::ComposeString::close() {
  write(closingStrings.top());
  closingStrings.pop();
}

inline void Utils::ComposeString::closeAll() {
  while (!closingStrings.empty()) {
    close();
  }
}

inline void Utils::ComposeString::setCapacity(const size_t newCap) {
  auto newStr = std::make_unique<char []>(newCap);
  std::memcpy(newStr.get(), string.get(), size);
  string = std::move(newStr);
  capacity = newCap;
}
