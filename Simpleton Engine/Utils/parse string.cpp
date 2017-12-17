//
//  parse string.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 30/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "parse string.hpp"

Utils::ParseStringExpectError::ParseStringExpectError(const char c)
  : std::runtime_error(std::string("Expected character '") + c + '\''),
    expected(c) {}

Utils::ParseStringNumberError::ParseStringNumberError(const std::string &error)
  : std::runtime_error("Error while parsing number: " + error) {}

char Utils::ParseStringExpectError::expectedChar() const {
  return expected;
}

Utils::ParseString::ParseString(const std::string &string)
  : mData(string.data()), mSize(string.size()) {}

Utils::ParseString::ParseString(const std::string_view view)
  : mData(view.data()), mSize(view.size()) {
  throwIfNull(view.data());
}

Utils::ParseString::ParseString(const char *data, const size_t size)
  : mData(data), mSize(size) {
  throwIfNull(data);
}

Utils::ParseString::LineCol Utils::ParseString::lineCol() const {
  return mLineCol;
}

std::string_view Utils::ParseString::view() const {
  return {mData, mSize};
}

std::string_view Utils::ParseString::view(const size_t numChars) const {
  if (numChars > mSize) {
    throw std::out_of_range("View size larger than string");
  }
  return {mData, numChars};
}

void Utils::ParseString::advance(const size_t numChars) {
  if (numChars > mSize) {
    throw std::out_of_range("Advanced parse string too many characters");
  }
  mLineCol.putString(mData, numChars);
  mData += numChars;
  mSize -= numChars;
}

void Utils::ParseString::advance() {
  if (mSize == 0) {
    throw std::out_of_range("Advanced parse string too many characters");
  }
  mLineCol.putChar(*mData);
  ++mData;
  --mSize;
}

void Utils::ParseString::skip(const char ch) {
  skip([ch] (const char c) {
    return c == ch;
  });
}

void Utils::ParseString::skipWhitespace() {
  skip(isspace);
}

void Utils::ParseString::skipUntil(const char ch) {
  skipUntil([ch] (const char c) {
    return c == ch;
  });
}

void Utils::ParseString::skipUntilWhitespace() {
  skipUntil(isspace);
}

void Utils::ParseString::expect(const char c) {
  if (mSize == 0 || *mData != c) {
    throw ParseStringExpectError(c);
  }
  advance();
}



void Utils::ParseString::expectAfterWhitespace(const char c) {
  skipWhitespace();
  expect(c);
}

bool Utils::ParseString::check(const char c) {
  if (mSize == 0 || *mData != c) {
    return false;
  } else {
    advance();
    return true;
  }
}

bool Utils::ParseString::check(const char *data, const size_t size) {
  if (mSize < size) {
    return false;
  }
  if (size == 0) {
    return true;
  }
  if (std::memcmp(mData, data, size) == 0) {
    advance(size);
    return true;
  } else {
    return false;
  }
}

bool Utils::ParseString::check(const std::string_view view) {
  return check(view.data(), view.size());
}

size_t Utils::ParseString::copy(char *const dst, const size_t dstSize) {
  throwIfNull(dst);
  const size_t numChars = mSize < dstSize ? mSize : dstSize;
  std::memcpy(dst, mData, numChars);
  advance(numChars);
  return numChars;
}

size_t Utils::ParseString::copyUntil(char *const dst, const size_t dstSize, const char ch) {
  return copyUntil(dst, dstSize, [ch] (const char c) {
    return ch == c;
  });
}

size_t Utils::ParseString::copyUntilWhitespace(char *const dst, const size_t dstSize) {
  return copyUntil(dst, dstSize, isspace);
}
