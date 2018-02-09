//
//  parse string.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 30/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

inline Utils::ParseStringExpectError::ParseStringExpectError(
  const char c,
  const unsigned line,
  const unsigned col)
  : mLine(line),
    mCol(col),
    mExpected(c) {}

inline char Utils::ParseStringExpectError::expectedChar() const {
  return mExpected;
}

inline unsigned Utils::ParseStringExpectError::line() const {
  return mLine;
}

inline unsigned Utils::ParseStringExpectError::column() const {
  return mCol;
}

inline const char *Utils::ParseStringExpectError::what() const noexcept {
  //@TODO use static memory when std::to_chars arrives
  return (std::string("Expected character '")
         + mExpected
         + "' at "
         + std::to_string(mLine)
         + ':'
         + std::to_string(mCol)).c_str();
}

inline Utils::ParseStringNumberError::ParseStringNumberError(const std::string &error)
  : std::runtime_error("Error while parsing number: " + error) {}

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
  if (numChars > mSize) {
    throw std::out_of_range("View size larger than string");
  }
  return {mData, numChars};
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

inline void Utils::ParseString::advance(const size_t numChars) {
  if (numChars > mSize) {
    throw std::out_of_range("Advanced parse string too many characters");
  }
  advanceNoCheck(numChars);
}

inline void Utils::ParseString::advance() {
  if (mSize == 0) {
    throw std::out_of_range("Advanced parse string too many characters");
  }
  advanceNoCheck();
}

inline void Utils::ParseString::skip(const char ch) {
  skip([ch] (const char c) {
    return c == ch;
  });
}

template <typename Pred>
void Utils::ParseString::skip(Pred &&pred) {
  size_t numChars = 0;
  while (numChars < mSize && pred(mData[numChars])) {
    ++numChars;
  }
  advanceNoCheck(numChars);
}

inline void Utils::ParseString::skipWhitespace() {
  skip(isspace);
}

inline void Utils::ParseString::skipUntil(const char ch) {
  skipUntil([ch] (const char c) {
    return c == ch;
  });
}

template <typename Pred>
void Utils::ParseString::skipUntil(Pred &&pred) {
  skip(std::not_fn(pred));
}

inline void Utils::ParseString::skipUntilWhitespace() {
  skipUntil(isspace);
}

inline void Utils::ParseString::expect(const char c) {
  if (mSize == 0 || *mData != c) {
    throw ParseStringExpectError(c, mLineCol.line(), mLineCol.col());
  }
  advanceNoCheck();
}

template <typename Pred>
void Utils::ParseString::expectAfter(Pred &&pred, const char c) {
  skip(pred);
  expect(c);
}

inline void Utils::ParseString::expectAfterWhitespace(const char c) {
  expectAfter(isspace, c);
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
void Utils::ParseString::parseNumber(Number &number) {
  if constexpr (std::is_integral<Number>::value) {
    if constexpr (std::is_unsigned<Number>::value) {
      char *end;
      const unsigned long long num = std::strtoull(mData, &end, 0);
      if (errno == ERANGE || num > std::numeric_limits<Number>::max()) {
        throw ParseStringNumberError("Number out of range");
      }
      if (num == 0 && end == mData) {
        throw ParseStringNumberError("Invalid number");
      }
      advanceNoCheck(end - mData);
      number = static_cast<Number>(num);
    } else if constexpr (std::is_signed<Number>::value) {
      char *end;
      const long long num = std::strtoll(mData, &end, 0);
      if (errno == ERANGE || num < std::numeric_limits<Number>::lowest() || num > std::numeric_limits<Number>::max()) {
        throw ParseStringNumberError("Number out of range");
      }
      if (num == 0 && end == mData) {
        throw ParseStringNumberError("Invalid number");
      }
      advanceNoCheck(end - mData);
      number = static_cast<Number>(num);
    }
  } else if constexpr (std::is_floating_point<Number>::value) {
    char *end;
    const long double num = std::strtold(mData, &end);
    if (errno == ERANGE || num < std::numeric_limits<Number>::lowest() || num > std::numeric_limits<Number>::max()) {
      throw ParseStringNumberError("Number out of range");
    }
    if (num == 0 && end == mData) {
      throw ParseStringNumberError("Invalid number");
    }
    advanceNoCheck(end - mData);
    number = static_cast<Number>(num);
  }

  /*
  @TODO
  
  const auto [end, error] = std::from_chars(mData, mData + mSize, number);
  if (error) {
    throw ParseStringNumberError(error.message());
  }
  advanceNoCheck(end - mData);
  */
}

template <typename Number>
Number Utils::ParseString::parseNumber() {
  Number number;
  parseNumber(number);
  return number;
}

inline size_t Utils::ParseString::copy(char *const dst, const size_t dstSize) {
  throwIfNull(dst);
  const size_t numChars = mSize < dstSize ? mSize : dstSize;
  std::memcpy(dst, mData, numChars);
  advanceNoCheck(numChars);
  return numChars;
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

inline size_t Utils::ParseString::copyUntil(char *const dst, const size_t dstSize, const char ch) {
  return copyUntil(dst, dstSize, [ch] (const char c) {
    return ch == c;
  });
}

template <typename Pred>
size_t Utils::ParseString::copyUntil(char *const dst, const size_t dstSize, Pred &&pred) {
  return copyWhile(dst, dstSize, std::not_fn(pred));
}

inline size_t Utils::ParseString::copyUntilWhitespace(char *const dst, const size_t dstSize) {
  return copyUntil(dst, dstSize, isspace);
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
