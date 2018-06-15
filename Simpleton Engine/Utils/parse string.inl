//
//  parse string.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 30/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include <cerrno>
#include "endian.hpp"
#include <functional>

namespace Utils {
  inline auto charEqualTo(const char ch) {
    return [ch] (const char c) -> bool {
      return c == ch;
    };
  }
}

inline Utils::ExpectChar::ExpectChar(
  const char c,
  const unsigned line,
  const unsigned col
) : mLine(line),
    mCol(col),
    mExpected(c) {}

inline char Utils::ExpectChar::expectedChar() const {
  return mExpected;
}

inline unsigned Utils::ExpectChar::line() const {
  return mLine;
}

inline unsigned Utils::ExpectChar::column() const {
  return mCol;
}

inline const char *Utils::ExpectChar::what() const noexcept {
  //@TODO use static memory when std::to_chars arrives
  return (
    std::string("Expected character '")
    + mExpected
    + "' at "
    + std::to_string(mLine)
    + ':'
    + std::to_string(mCol)
  ).c_str();
}

inline Utils::ExpectString::ExpectString(
  const char *data,
  const size_t size,
  const unsigned line,
  const unsigned col
) : mLine(line), mCol(col) {
  mExpectedSize = size < MAX_STR_SIZE ? size : MAX_STR_SIZE;
  std::memcpy(mExpected, data, mExpectedSize);
}

inline std::string_view Utils::ExpectString::expectedStr() const {
  return {mExpected, mExpectedSize};
}

inline unsigned Utils::ExpectString::line() const {
  return mLine;
}

inline unsigned Utils::ExpectString::column() const {
  return mCol;
}

inline const char *Utils::ExpectString::what() const noexcept {
  //@TODO use static memory when std::to_chars arrives
  return (
    std::string("Expected string at ")
    + std::to_string(mLine)
    + ':'
    + std::to_string(mCol)
    + " \""
    + std::string(mExpected, mExpectedSize)
    + '"'
  ).c_str();
}

inline Utils::InvalidNumber::InvalidNumber(
  const std::error_code error,
  const unsigned line,
  const unsigned col
) : mError{error}, mLine{line}, mCol{col} {}

inline Utils::InvalidNumber::InvalidNumber(
  const std::errc error,
  const unsigned line,
  const unsigned col
) : mError{std::make_error_code(error)}, mLine{line}, mCol{col} {}

inline std::error_code Utils::InvalidNumber::error() const {
  return mError;
}

inline unsigned Utils::InvalidNumber::line() const {
  return mLine;
}

inline unsigned Utils::InvalidNumber::column() const {
  return mCol;
}

inline const char *Utils::InvalidNumber::what() const noexcept {
  //@TODO use static memory when std::to_chars arrives
  return (
    std::string("Expected number at ")
    + std::to_string(mLine)
    + ':'
    + std::to_string(mCol)
    + " \""
    + mError.message()
    + '"'
  ).c_str();
}

inline Utils::ParseString::ParseString(const std::string &string)
  : mData(string.data()), mSize(string.size()) {}

inline Utils::ParseString::ParseString(const std::string_view view)
  : mData(view.data()), mSize(view.size()) {
  throwIfNull(view.data());
}

inline Utils::ParseString::ParseString(const char *data, const size_t size)
  : mData(data), mSize(size) {
  throwIfNull(data);
}

inline const char *Utils::ParseString::data() const {
  return mData;
}

inline size_t Utils::ParseString::size() const {
  return mSize;
}

inline Utils::ParseString::LineCol Utils::ParseString::lineCol() const {
  return mLineCol;
}

inline std::string_view Utils::ParseString::view() const {
  return {mData, mSize};
}

inline std::string_view Utils::ParseString::view(const size_t numChars) const {
  return {mData, mSize < numChars ? mSize : numChars};
}

inline bool Utils::ParseString::empty() const {
  return mSize == 0;
}

inline char Utils::ParseString::operator[](const size_t i) const {
  return mData[i];
}

inline char Utils::ParseString::at(const size_t i) const {
  if (i >= mSize) {
    throw std::out_of_range("Index on parse string out of range");
  }
  return mData[i];
}

inline char Utils::ParseString::front() const {
  return *mData;
}

inline Utils::ParseString &Utils::ParseString::advance(const size_t numChars) {
  if (numChars > mSize) {
    throw std::out_of_range("Advanced parse string too many characters");
  }
  advanceNoCheck(numChars);
  return *this;
}

inline Utils::ParseString &Utils::ParseString::advance() {
  if (mSize == 0) {
    throw std::out_of_range("Advanced parse string too many characters");
  }
  advanceNoCheck();
  return *this;
}

inline Utils::ParseString &Utils::ParseString::skip(const char c) {
  skip(charEqualTo(c));
  return *this;
}

template <typename Pred>
Utils::ParseString &Utils::ParseString::skip(Pred &&pred) {
  size_t numChars = 0;
  while (numChars < mSize && pred(mData[numChars])) {
    ++numChars;
  }
  advanceNoCheck(numChars);
  return *this;
}

inline Utils::ParseString &Utils::ParseString::skipWhitespace() {
  return skip(isspace);
}

inline Utils::ParseString &Utils::ParseString::skipUntil(const char c) {
  return skipUntil(charEqualTo(c));
}

template <typename Pred>
Utils::ParseString &Utils::ParseString::skipUntil(Pred &&pred) {
  return skip(std::not_fn(pred));
}

inline Utils::ParseString &Utils::ParseString::skipUntilWhitespace() {
  return skipUntil(isspace);
}

inline Utils::ParseString &Utils::ParseString::expect(const char c) {
  if (mSize == 0 || *mData != c) {
    throw ExpectChar(c, mLineCol.line(), mLineCol.col());
  }
  advanceNoCheck();
  return *this;
}

inline Utils::ParseString &Utils::ParseString::expect(const char *data, const size_t size) {
  if (mSize < size || std::memcmp(mData, data, size) != 0) {
    throw ExpectString(data, size, mLineCol.line(), mLineCol.col());
  }
  advanceNoCheck(size);
  return *this;
}

inline Utils::ParseString &Utils::ParseString::expect(const std::string_view view) {
  return expect(view.data(), view.size());
}

template <typename Pred>
inline Utils::ParseString &Utils::ParseString::expect(Pred &&pred, const std::string_view name) {
  // @TODO new exception class just for this function?
  if (mSize == 0 || !pred(*mData)) {
    throw ExpectString(name.data(), name.size(), mLineCol.line(), mLineCol.col());
  }
  advanceNoCheck();
  return *this;
}

template <typename Pred>
inline Utils::ParseString &Utils::ParseString::expectAfter(const char c, Pred &&pred) {
  return skip(pred).expect(c);
}

inline Utils::ParseString &Utils::ParseString::expectAfterWhitespace(const char c) {
  return skipWhitespace().expect(c);
}

inline bool Utils::ParseString::check(const char c) {
  if (mSize == 0 || *mData != c) {
    return false;
  } else {
    advanceNoCheck();
    return true;
  }
}

inline bool Utils::ParseString::check(const char *data, const size_t size) {
  if (mSize < size) {
    return false;
  }
  if (size == 0) {
    return true;
  }
  if (std::memcmp(mData, data, size) == 0) {
    advanceNoCheck(size);
    return true;
  } else {
    return false;
  }
}

inline bool Utils::ParseString::check(const std::string_view view) {
  return check(view.data(), view.size());
}

template <size_t SIZE>
bool Utils::ParseString::check(const char (&string)[SIZE]) {
  return check(string, SIZE);
}

template <typename Pred>
bool Utils::ParseString::check(Pred &&pred) {
  if (mSize == 0 || !pred(*mData)) {
    return false;
  } else {
    advanceNoCheck();
    return true;
  }
}

template <typename Number>
std::error_code Utils::ParseString::tryParseNumber(Number &number) {
  if constexpr (std::is_integral<Number>::value) {
    if constexpr (std::is_unsigned<Number>::value) {
      char *end;
      const unsigned long long num = std::strtoull(mData, &end, 0);
      if (errno == ERANGE || num > std::numeric_limits<Number>::max()) {
        return std::make_error_code(std::errc::result_out_of_range);
      }
      if (num == 0 && end == mData) {
        return std::make_error_code(std::errc::invalid_argument);
      }
      advanceNoCheck(end - mData);
      number = static_cast<Number>(num);
    } else if constexpr (std::is_signed<Number>::value) {
      char *end;
      const long long num = std::strtoll(mData, &end, 0);
      if (errno == ERANGE || num < std::numeric_limits<Number>::lowest() || num > std::numeric_limits<Number>::max()) {
        return std::make_error_code(std::errc::result_out_of_range);
      }
      if (num == 0 && end == mData) {
        return std::make_error_code(std::errc::invalid_argument);
      }
      advanceNoCheck(end - mData);
      number = static_cast<Number>(num);
    }
  } else if constexpr (std::is_floating_point<Number>::value) {
    char *end;
    const long double num = std::strtold(mData, &end);
    if (errno == ERANGE || num < std::numeric_limits<Number>::lowest() || num > std::numeric_limits<Number>::max()) {
      return std::make_error_code(std::errc::result_out_of_range);
    }
    if (num == 0 && end == mData) {
      return std::make_error_code(std::errc::invalid_argument);
    }
    advanceNoCheck(end - mData);
    number = static_cast<Number>(num);
  }
  
  return std::error_code{};
  
  /*
  @TODO std::from_chars
  
  const auto [end, error] = std::from_chars(mData, mData + mSize, number);
  if (!error) {
    advanceNoCheck(end - mData);
  }
  return error;
  */
}

template <typename Number>
Utils::ParseString &Utils::ParseString::parseNumber(Number &number) {
  const std::error_code error = tryParseNumber(number);
  if (error) {
    throw InvalidNumber(error, mLineCol.line(), mLineCol.col());
  }
  return *this;
}

template <typename Number>
Number Utils::ParseString::parseNumber() {
  Number number;
  parseNumber(number);
  return number;
}

template <typename Number>
Utils::ParseString &Utils::ParseString::readNumberLil(Number &n) {
  if (mSize < sizeof(Number)) {
    throw InvalidNumber(std::errc::no_buffer_space, mLineCol.line(), mLineCol.col());
  }
  copyFromLilEndian(&n, mData, 1);
  advanceBin(sizeof(Number));
  return *this;
}

template <typename Number>
Utils::ParseString &Utils::ParseString::readNumberBig(Number &n) {
  if (mSize < sizeof(Number)) {
    throw InvalidNumber(std::errc::no_buffer_space, mLineCol.line(), mLineCol.col());
  }
  copyFromBigEndian(&n, mData, 1);
  advanceBin(sizeof(Number));
  return *this;
}

template <typename Number>
Utils::ParseString &Utils::ParseString::readNumbersLil(Number *n, const size_t size) {
  if (mSize < sizeof(Number) * size) {
    throw InvalidNumber(std::errc::no_buffer_space, mLineCol.line(), mLineCol.col());
  }
  copyFromLilEndian(n, mData, size);
  advanceBin(sizeof(Number) * size);
  return *this;
}

template <typename Number>
Utils::ParseString &Utils::ParseString::readNumbersBig(Number *n, const size_t size) {
  if (mSize < sizeof(Number) * size) {
    throw InvalidNumber(std::errc::no_buffer_space, mLineCol.line(), mLineCol.col());
  }
  copyFromBigEndian(n, mData, size);
  advanceBin(sizeof(Number) * size);
  return *this;
}

inline size_t Utils::ParseString::parseEnum(const std::string_view *const names, const size_t numNames) {
  throwIfNull(names);
  const std::string_view *const end = names + numNames;
  for (const std::string_view *n = names; n != end; ++n) {
    if (check(*n)) {
      return n - names;
    }
  }
  return numNames;
}

inline size_t Utils::ParseString::copy(char *const dst, const size_t dstSize) {
  throwIfNull(dst);
  const size_t numChars = mSize < dstSize ? mSize : dstSize;
  std::memcpy(dst, mData, numChars);
  advanceNoCheck(numChars);
  return numChars;
}

inline Utils::ParseString &Utils::ParseString::copy(std::string &dst, const size_t copySize) {
  const size_t numChars = mSize < copySize ? mSize : copySize;
  dst.append(mData, numChars);
  advanceNoCheck(numChars);
  return *this;
}

template <typename Pred>
size_t Utils::ParseString::copyWhile(char *dst, const size_t dstSize, Pred &&pred) {
  throwIfNull(dst);
  size_t numChars = 0;
  const size_t maxChars = mSize < dstSize ? mSize : dstSize;
  while (numChars < maxChars && pred(*mData)) {
    *dst = *mData;
    ++dst;
    advanceNoCheck(); // increments mData
    ++numChars;
  }
  return numChars;
}

template <typename Pred>
Utils::ParseString &Utils::ParseString::copyWhile(std::string &dst, Pred &&pred) {
  while (mSize && pred(*mData)) {
    dst.push_back(*mData);
    advanceNoCheck();
  }
  return *this;
}

inline size_t Utils::ParseString::copyUntil(char *const dst, const size_t dstSize, const char c) {
  return copyUntil(dst, dstSize, charEqualTo(c));
}

inline Utils::ParseString &Utils::ParseString::copyUntil(std::string &dst, const char c) {
  return copyUntil(dst, charEqualTo(c));
}

template <typename Pred>
size_t Utils::ParseString::copyUntil(char *const dst, const size_t dstSize, Pred &&pred) {
  return copyWhile(dst, dstSize, std::not_fn(pred));
}

template <typename Pred>
Utils::ParseString &Utils::ParseString::copyUntil(std::string &dst, Pred &&pred) {
  return copyWhile(dst, std::not_fn(pred));
}

inline size_t Utils::ParseString::copyUntilWhitespace(char *const dst, const size_t dstSize) {
  return copyUntil(dst, dstSize, isspace);
}

inline Utils::ParseString &Utils::ParseString::copyUntilWhitespace(std::string &dst) {
  return copyUntil(dst, isspace);
}

inline void Utils::ParseString::advanceNoCheck(const size_t numChars) {
  mLineCol.putString(mData, numChars);
  mData += numChars;
  mSize -= numChars;
}

inline void Utils::ParseString::advanceNoCheck() {
  mLineCol.putChar(*mData);
  ++mData;
  --mSize;
}

inline void Utils::ParseString::advanceBin(const size_t numChars) {
  mData += numChars;
  mSize -= numChars;
}
