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
    void skip(Pred &&);
    ///Move the front forward while the front is whitespace
    void skipWhitespace();
    
    ///Move the front forward until the front is equal to the supplied character
    void skipUntil(char);
    ///Move the front forward until the supplied predicate returns true
    template <typename Pred>
    void skipUntil(Pred &&);
    ///Move the front forward until the front is whitespace
    void skipUntilWhitespace();
    
    ///Throw a ParseStringExpectError exception if the front character is not
    ///equal to the supplied character
    void expect(char);
    ///Skip characters then throw a ParseStringExpectError exception if the
    ///front character is not equal to the supplied character
    template <typename Pred>
    void expectAfter(Pred &&, char);
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
    bool check(Pred &&);
    
    ///Interprets the front part of the string as a number. Throws a
    ///ParseStringNumberError exception on failure
    template <typename Number>
    void parseNumber(Number &);
    ///Interprets the front part of the string as a number. Throws a
    ///ParseStringNumberError exception on failure
    template <typename Number>
    Number parseNumber();
    
    ///Copies characters from the front part of the string
    size_t copy(char *, size_t);
    
    ///Copies characters from the front part of the string while the supplied
    ///predicate returns true. Advances the number of characters that
    ///were copied.
    template <typename Pred>
    size_t copyWhile(char *, size_t, Pred &&);
    
    ///Copies characters from the front part of the string until the front is
    ///equal to the supplied character
    size_t copyUntil(char *, size_t, char);
    ///Copies characters from the front part of the string until the supplied
    ///predicate returns true. Advances the number of characters that
    ///were copied.
    template <typename Pred>
    size_t copyUntil(char *, size_t, Pred &&);
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
}

#include "parse string.inl"

#endif
