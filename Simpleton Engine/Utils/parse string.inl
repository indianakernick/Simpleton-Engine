//
//  parse string.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 30/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include <cerrno>
#include <functional>

namespace Utils {
  inline auto charEqualTo(const char ch) {
    return [ch] (const char c) -> bool {
      return c == ch;
    };
  }
}

inline Utils::ParsingError::ParsingError(
  const LineCol<> lineCol
) : mLine(lineCol.line()),
    mCol(lineCol.col()) {}

inline unsigned Utils::ParsingError::line() const {
  return mLine;
}

inline unsigned Utils::ParsingError::column() const {
  return mCol;
}

inline Utils::ExpectChar::ExpectChar(
  const char c,
  const LineCol<> lineCol
) : ParsingError(lineCol), mExpected(c) {}

inline char Utils::ExpectChar::expectedChar() const {
  return mExpected;
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
  const std::string_view string,
  const LineCol<> lineCol
) : ParsingError(lineCol) {
  mExpectedSize = string.size() < MAX_STR_SIZE ? string.size() : MAX_STR_SIZE;
  std::memcpy(mExpected, string.data(), mExpectedSize);
}

inline std::string_view Utils::ExpectString::expectedStr() const {
  return {mExpected, mExpectedSize};
}

inline const char *Utils::ExpectString::what() const noexcept {
  //@TODO use static memory
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
  const LineCol<> lineCol
) : ParsingError{lineCol}, mError{error} {}

inline Utils::InvalidNumber::InvalidNumber(
  const std::errc error,
  const LineCol<> lineCol
) : InvalidNumber{std::make_error_code(error), lineCol} {}

inline std::error_code Utils::InvalidNumber::error() const {
  return mError;
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

inline Utils::InvalidEnum::InvalidEnum(
  const std::string_view name,
  const LineCol<> lineCol
) : ParsingError{lineCol} {
  size_ = name.size() < MAX_STR_SIZE ? name.size() : MAX_STR_SIZE;
  std::memcpy(name_, name.data(), size_);
}

inline std::string_view Utils::InvalidEnum::name() const {
  return {name_, size_};
}

inline const char *Utils::InvalidEnum::what() const noexcept {
  //@TODO use static memory
  return (
    std::string("Expected enumeration at ")
    + std::to_string(mLine)
    + ':'
    + std::to_string(mCol)
    + " No matching enumeration for \""
    + std::string(name_, size_)
    + '"'
  ).c_str();
}

inline Utils::ParseString::ParseString(const std::string &string)
  : beg{string.data()}, end{string.data() + string.size()} {}

inline Utils::ParseString::ParseString(const std::string_view view)
  : beg{view.data()}, end{view.data() + view.size()} {
  throwIfNull(view.data());
}

inline Utils::ParseString::ParseString(const char *const data, const size_t size)
  : beg{data}, end{data + size} {
  throwIfNull(data);
}

inline const char *Utils::ParseString::data() const {
  return beg;
}

inline size_t Utils::ParseString::size() const {
  return end - beg;
}

inline Utils::LineCol<> Utils::ParseString::lineCol() const {
  return pos;
}

inline std::string_view Utils::ParseString::view() const {
  return {beg, size()};
}

inline std::string_view Utils::ParseString::view(const size_t numChars) const {
  return {beg, minSize(numChars)};
}

inline bool Utils::ParseString::empty() const {
  return beg == end;
}

inline char Utils::ParseString::operator[](const size_t i) const {
  return beg[i];
}

inline char Utils::ParseString::at(const size_t i) const {
  const char *const ptr = beg + i;
  if (ptr >= end) {
    throw std::out_of_range("Index on parse string out of range");
  }
  return *ptr;
}

inline char Utils::ParseString::front() const {
  return *beg;
}

inline Utils::ParseString &Utils::ParseString::advance(const size_t numChars) {
  if (beg + numChars >= end) {
    throw std::out_of_range("Advanced parse string too many characters");
  }
  advanceNoCheck(numChars);
  return *this;
}

inline Utils::ParseString &Utils::ParseString::advance() {
  if (empty()) {
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
  while (beg < end && pred(*beg)) {
    advanceNoCheck();
  }
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
  if (empty() || *beg != c) {
    throw ExpectChar(c, pos);
  }
  advanceNoCheck();
  return *this;
}

inline Utils::ParseString &Utils::ParseString::expect(const char *data, const size_t bufSize) {
  if (size() < bufSize || std::memcmp(beg, data, bufSize) != 0) {
    throw ExpectString({data, bufSize}, pos);
  }
  advanceNoCheck(bufSize);
  return *this;
}

inline Utils::ParseString &Utils::ParseString::expect(const std::string_view view) {
  return expect(view.data(), view.size());
}

template <typename Pred>
inline Utils::ParseString &Utils::ParseString::expect(Pred &&pred, const std::string_view name) {
  // @TODO new exception class just for this function?
  if (empty() || !pred(*beg)) {
    throw ExpectString(name, pos);
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
  if (empty() || *beg != c) {
    return false;
  } else {
    advanceNoCheck();
    return true;
  }
}

inline bool Utils::ParseString::check(const char *data, const size_t size) {
  if (size == 0) {
    return true;
  }
  if (end <= data + size) {
    return false;
  }
  if (std::memcmp(beg, data, size) == 0) {
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
  if (empty() || !pred(*beg)) {
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
      const unsigned long long num = std::strtoull(beg, &end, 0);
      if (errno == ERANGE || num > std::numeric_limits<Number>::max()) {
        return std::make_error_code(std::errc::result_out_of_range);
      }
      if (num == 0 && end == beg) {
        return std::make_error_code(std::errc::invalid_argument);
      }
      advanceNoCheck(end - beg);
      number = static_cast<Number>(num);
    } else if constexpr (std::is_signed<Number>::value) {
      char *end;
      const long long num = std::strtoll(beg, &end, 0);
      if (errno == ERANGE || num < std::numeric_limits<Number>::lowest() || num > std::numeric_limits<Number>::max()) {
        return std::make_error_code(std::errc::result_out_of_range);
      }
      if (num == 0 && end == beg) {
        return std::make_error_code(std::errc::invalid_argument);
      }
      advanceNoCheck(end - beg);
      number = static_cast<Number>(num);
    }
  } else if constexpr (std::is_floating_point<Number>::value) {
    char *end;
    const long double num = std::strtold(beg, &end);
    if (errno == ERANGE || num < std::numeric_limits<Number>::lowest() || num > std::numeric_limits<Number>::max()) {
      return std::make_error_code(std::errc::result_out_of_range);
    }
    if (num == 0 && end == beg) {
      return std::make_error_code(std::errc::invalid_argument);
    }
    advanceNoCheck(end - beg);
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
    throw InvalidNumber(error, pos);
  }
  return *this;
}

template <typename Number>
Number Utils::ParseString::parseNumber() {
  Number number;
  parseNumber(number);
  return number;
}

inline size_t Utils::ParseString::tryParseEnum(
  const std::string_view *const names,
  const size_t numNames
) {
  throwIfNull(names);
  const std::string_view *const end = names + numNames;
  for (const std::string_view *n = names; n != end; ++n) {
    if (check(*n)) {
      return n - names;
    }
  }
  return numNames;
}

template <typename Enum>
Enum Utils::ParseString::parseEnum(
  const std::string_view *const names,
  const size_t numNames
) {
  const size_t index = tryParseEnum(names, numNames);
  if (index == numNames) {
    throw InvalidEnum(view(), pos);
  }
  return static_cast<Enum>(index);
}

inline size_t Utils::ParseString::copy(char *const dst, const size_t dstSize) {
  throwIfNull(dst);
  const size_t numChars = minSize(dstSize);
  std::memcpy(dst, beg, numChars);
  advanceNoCheck(numChars);
  return numChars;
}

inline Utils::ParseString &Utils::ParseString::copy(std::string &dst, const size_t copySize) {
  const size_t numChars = minSize(copySize);
  dst.append(beg, numChars);
  advanceNoCheck(numChars);
  return *this;
}

template <typename Pred>
size_t Utils::ParseString::copyWhile(char *dst, const size_t dstSize, Pred &&pred) {
  throwIfNull(dst);
  size_t numChars = 0;
  const size_t maxChars = minSize(dstSize);
  while (numChars < maxChars && pred(*beg)) {
    *dst = *beg;
    ++dst;
    advanceNoCheck(); // increments mData
    ++numChars;
  }
  return numChars;
}

template <typename Pred>
Utils::ParseString &Utils::ParseString::copyWhile(std::string &dst, Pred &&pred) {
  while (!empty() && pred(*beg)) {
    dst.push_back(*beg);
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
  pos.putString(beg, numChars);
  beg += numChars;
}

inline void Utils::ParseString::advanceNoCheck() {
  pos.putChar(*beg);
  ++beg;
}

inline size_t Utils::ParseString::minSize(const size_t otherSize) const {
  const size_t thisSize = size();
  return thisSize < otherSize ? thisSize : otherSize;
}
