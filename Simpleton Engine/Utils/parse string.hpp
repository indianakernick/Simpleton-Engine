//
//  parse string.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 30/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_parse_string_hpp
#define engine_utils_parse_string_hpp

#include <cerrno>
#include <functional>
#include <string_view>
#include "line col.hpp"

namespace Utils {
  ///A character was expected but not present
  class ParseStringExpectError final : public std::exception {
  public:
    ParseStringExpectError(char, unsigned, unsigned);
  
    char expectedChar() const;
    unsigned line() const;
    unsigned column() const;
    
    const char* what() const noexcept override;
    
  private:
    unsigned mLine;
    unsigned mCol;
    char mExpected;
  };
  
  ///Unable to parse number
  class ParseStringNumberError final : public std::runtime_error {
  public:
    explicit ParseStringNumberError(const std::string &);
  };

  ///A view onto a string being parsed
  class ParseString {
  public:
    using LineCol = LineCol<unsigned, unsigned>;
  
    explicit ParseString(const std::string &);
    explicit ParseString(std::string_view);
    ParseString(const char *, size_t);
    
    const char *data() const;
    size_t size() const;
    LineCol lineCol() const;
    std::string_view view() const;
    std::string_view view(size_t) const;
    
    ///Returns true if the string is empty
    bool empty() const;
    ///Get a character on the string. Does not do range checking
    char operator[](size_t) const;
    ///Get a character on the string. Does range checking
    char at(size_t) const;
    ///Get the first character. Does not check the size of the string
    char front() const;
    
    ///Move the front of the string forward. Increments line and column
    ///numbers accordingly
    void advance(size_t);
    ///Move the front of the string forward by one character. Increments line
    ///and column numbers accordingly
    void advance();
    
    ///Move the front forward while the front is equal to the supplied character
    void skip(char);
    ///Move the front forward while the supplied predicate returns true
    template <typename Pred>
    void skip(Pred &&pred) {
      size_t numChars = 0;
      while (numChars < mSize && pred(mData[numChars])) {
        ++numChars;
      }
      advanceNoCheck(numChars);
    }
    ///Move the front forward while the front is whitespace
    void skipWhitespace();
    
    ///Move the front forward until the front is equal to the supplied character
    void skipUntil(char);
    ///Move the front forward until the supplied predicate returns true
    template <typename Pred>
    void skipUntil(Pred &&pred) {
      skip(std::not_fn(pred));
    }
    ///Move the front forward until the front is whitespace
    void skipUntilWhitespace();
    
    ///Throw a ParseStringExpectError exception if the front character is not
    ///equal to the supplied character
    void expect(char);
    ///Skip characters then throw a ParseStringExpectError exception if the
    ///front character is not equal to the supplied character
    template <typename Pred>
    void expectAfter(Pred &&pred, const char c) {
      skip(pred);
      expect(c);
    }
    ///Skip whitespace then throw a ParseStringExpectError exception if the
    ///front character is not equal to the supplied character
    void expectAfterWhitespace(char);
    
    ///Advances and returns true if the front character is equal to the supplied
    ///character. Does nothing and returns false otherwise
    bool check(char);
    ///Advances and returns true if the front part of the string is equal to the
    ///supplied string. Does nothing and returns false otherwise
    bool check(const char *, size_t);
    ///Advances and returns true if the front part of the string is equal to the
    ///supplied string. Does nothing and returns false otherwise
    bool check(std::string_view);
    ///Advances and returns true if the supplied predicate returns true for the
    ///first character. Does nothing and returns false otherwise
    template <typename Pred>
    bool check(Pred &&pred) {
      if (mSize == 0 || !pred(*mData)) {
        return false;
      } else {
        advanceNoCheck();
        return true;
      }
    }
    
    ///Interprets the front part of the string as a number. Throws a
    ///ParseStringNumberError exception on failure
    template <typename Number>
    void parseNumber(Number &number) {
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
        const float num = std::strtold(mData, &end);
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
      const size_t numRead = end - mData;
      lineCol.putString(mData, numRead);
      mSize -= numRead;
      mData = end;
      */
    }
    
    ///Interprets the front part of the string as a number. Throws a
    ///ParseStringNumberError exception on failure
    template <typename Number>
    Number parseNumber() {
      Number number;
      parseNumber(number);
      return number;
    }
    
    ///Copies characters from the front part of the string
    size_t copy(char *, size_t);
    
    ///Copies characters from the front part of the string while the supplied
    ///predicate returns true. Advances the number of characters that
    ///were copied.
    template <typename Pred>
    size_t copyWhile(char *dst, const size_t dstSize, Pred &&pred) {
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
    
    ///Copies characters from the front part of the string until the front is
    ///equal to the supplied character
    size_t copyUntil(char *, size_t, char);
    ///Copies characters from the front part of the string until the supplied
    ///predicate returns true. Advances the number of characters that
    ///were copied.
    template <typename Pred>
    size_t copyUntil(char *const dst, const size_t dstSize, Pred &&pred) {
      return copyWhile(dst, dstSize, std::not_fn(pred));
    }
    ///Copies characters from the front part of the string until the front is
    ///whitespace. Advances the number of characters that were copied.
    size_t copyUntilWhitespace(char *, size_t);
    
  private:
    const char *mData;
    size_t mSize;
    LineCol mLineCol;
    
    //Advance without range checks
    void advanceNoCheck(size_t);
    void advanceNoCheck();
  };
  
  // "hot" functions
  
  inline const char *ParseString::data() const {
    return mData;
  }
  
  inline size_t ParseString::size() const {
    return mSize;
  }
  
  inline bool ParseString::empty() const {
    return mSize == 0;
  }
  
  inline char ParseString::operator[](const size_t i) const {
    return mData[i];
  }
  
  inline char ParseString::at(const size_t i) const {
    if (i >= mSize) {
      throw std::out_of_range("Index on parse string out of range");
    }
    return mData[i];
  }

  inline char ParseString::front() const {
    return *mData;
  }
}

#endif
