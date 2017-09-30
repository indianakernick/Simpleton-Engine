//
//  parse string.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 30/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_parse_string_hpp
#define engine_utils_parse_string_hpp

#include "line col.hpp"
#include <experimental/string_view>

namespace Utils {
  ///A character was expected but not present
  class ParseStringExpectError final : public std::runtime_error {
  public:
    explicit ParseStringExpectError(char);
  
    char expectedChar() const;
    
  private:
    char expected;
  };

  ///A view onto a string being parsed
  class ParseString {
  public:
    using LineCol = Utils::LineCol<unsigned, unsigned>;
  
    explicit ParseString(const std::string &);
    explicit ParseString(std::experimental::string_view);
    ParseString(const char *, size_t);
    
    const char *data() const;
    size_t size() const;
    LineCol lineCol() const;
    std::experimental::string_view view() const;
    std::experimental::string_view view(size_t) const;
    
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
    
    ///Move the front forward while the supplied predicate returns true
    template <typename Pred>
    void skip(Pred &&pred) {
      size_t numChars = 0;
      while (numChars < mSize && pred(mData[numChars])) {
        ++numChars;
      }
      advance(numChars);
    }
    ///Move the front forward while the front is whitespace
    void skipWhitespace();
    
    ///Move the front forward until the supplied predicate returns true
    template <typename Pred>
    void skipUntil(Pred &&pred) {
      //@TODO
      //skip(std::not_fn(pred));
      skip([pred = std::forward<Pred>(pred)](const char c) {
        return !pred(c);
      });
    }
    ///Move the front forward until the front is equal to the supplied character
    void skipUntil(char);
    
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
    bool check(std::experimental::string_view);
    
  private:
    const char *mData;
    size_t mSize;
    LineCol mLineCol;
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
