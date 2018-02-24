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
  const auto swapped = toLilEndian(num);
  write(reinterpret_cast<const char *>(&swapped), sizeof(Number));
}

template <typename Number>
void Utils::ComposeString::writeNumberBig(const Number num) {
  reserveToFit(sizeof(Number));
  const auto swapped = toBigEndian(num);
  write(reinterpret_cast<const char *>(&swapped), sizeof(Number));
}

template <typename Number>
void Utils::ComposeString::writeNumbersLil(const Number *n, const size_t s) {
  reserveToFit(sizeof(size_t) + sizeof(Number) * s);
  copyToLilEndian(curr(), &s, 1);
  copyToLilEndian(curr() + sizeof(size_t), n, s);
  addSize(sizeof(size_t) + sizeof(Number) * s);
}

template <typename Number>
void Utils::ComposeString::writeNumbersBig(const Number *n, const size_t s) {
  reserveToFit(sizeof(size_t) + sizeof(Number) * s);
  copyToBigEndian(curr(), &s, 1);
  copyToBigEndian(curr() + sizeof(size_t), n, s);
  addSize(sizeof(size_t) + sizeof(Number) * s);
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
  if (!closingStrings.empty()) {
    write(closingStrings.top());
    closingStrings.pop();
  }
}

inline void Utils::ComposeString::closeAll() {
  while (!closingStrings.empty()) {
    write(closingStrings.top());
    closingStrings.pop();
  }
}

inline void Utils::ComposeString::setCapacity(const size_t newCap) {
  auto newStr = std::make_unique<char []>(newCap);
  std::memcpy(newStr.get(), string.get(), size);
  string = std::move(newStr);
  capacity = newCap;
}
