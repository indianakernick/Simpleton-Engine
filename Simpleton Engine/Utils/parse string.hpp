//
//  parse string.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 30/9/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_parse_string_hpp
#define engine_utils_parse_string_hpp

#include <string>
#include <string_view>
#include "line col.hpp"

namespace Utils {
  class ParsingError : public std::exception {
  public:
    ParsingError(unsigned, unsigned);
    
    unsigned line() const;
    unsigned column() const;
  
  protected:
    const unsigned mLine;
    const unsigned mCol;
  };

  ///A character was expected but not present
  class ExpectChar final : public ParsingError {
  public:
    ExpectChar(char, unsigned, unsigned);
  
    char expectedChar() const;
    
    const char *what() const noexcept override;
    
  private:
    char mExpected;
  };
  
  ///A sequence of characters was expected but not present
  class ExpectString final : public ParsingError {
  public:
    ExpectString(std::string_view, unsigned, unsigned);
    
    std::string_view expectedStr() const;
    
    const char *what() const noexcept override;
  
  private:
    static constexpr size_t MAX_STR_SIZE = 64;
    char mExpected[MAX_STR_SIZE];
    size_t mExpectedSize;
  };
  
  ///Unable to interpret characters as a number
  class InvalidNumber final : public ParsingError {
  public:
    InvalidNumber(std::error_code, unsigned, unsigned);
    InvalidNumber(std::errc, unsigned, unsigned);
  
    std::error_code error() const;
  
    const char *what() const noexcept override;
  
  private:
    std::error_code mError;
  };
  
  ///Unable to interpret characters as an enumeration
  class InvalidEnum final : public ParsingError {
  public:
    InvalidEnum(std::string_view, unsigned, unsigned);
    
    std::string_view name() const;
    
    const char *what() const noexcept override;
  
  private:
    static constexpr size_t MAX_STR_SIZE = 64;
    char name_[MAX_STR_SIZE];
    size_t size_;
  };

  ///A view onto a string being parsed
  class ParseString {
  public:
    using LineCol = LineCol<unsigned, unsigned>;
    
    explicit ParseString(const std::string &);
    explicit ParseString(std::string_view);
    ParseString(const char *, size_t);
    
    ///Get a pointer to the string being parsed. This pointer is incremented
    ///as the string is parsed.
    const char *data() const;
    ///Get the size of the string that has not been parsed yet. This number
    ///is decremented as the string is parsed.
    size_t size() const;
    ///Get the line and column position of the string yet to be parsed
    ///relative to the beginning.
    LineCol lineCol() const;
    ///Get a std::string_view of the unparsed string
    std::string_view view() const;
    ///Get a std::string_view of the unparsed string no larger than the size
    ///provided. Returned size may be less than requested size to avoid
    ///running off the end of the string.
    std::string_view view(size_t) const;
    
    ///Return true if the string is empty
    bool empty() const;
    ///Get a character on the string. Does not do range checking
    char operator[](size_t) const;
    ///Get a character on the string. Does range checking
    char at(size_t) const;
    ///Get the first character. Does not check the size of the string
    char front() const;
    
    ///Move the front of the string forward. Increments line and column
    ///numbers accordingly
    ParseString &advance(size_t);
    ///Move the front of the string forward by one character. Increments line
    ///and column numbers accordingly
    ParseString &advance();
    
    ///Move the front forward while the front is equal to the supplied character
    ParseString &skip(char);
    ///Move the front forward while the supplied predicate returns true
    template <typename Pred>
    ParseString &skip(Pred &&);
    ///Move the front forward while the front is whitespace
    ParseString &skipWhitespace();
    
    ///Move the front forward until the front is equal to the supplied character
    ParseString &skipUntil(char);
    ///Move the front forward until the supplied predicate returns true
    template <typename Pred>
    ParseString &skipUntil(Pred &&);
    ///Move the front forward until the front is whitespace
    ParseString &skipUntilWhitespace();
    
    ///Throw a ExpectChar exception if the front character is not equal to the
    ///supplied character
    ParseString &expect(char);
    ///Throw a ExpectString exception if the front part of the string is not
    ///equal to the supplied string
    ParseString &expect(const char *, size_t);
    ///Throw a ExpectString exception if the front part of the string is not
    ///equal to the supplied string
    ParseString &expect(std::string_view);
    ///Throw a ExpectString exception if the supplied predicate returns false.
    ///The name of the predicate is copied into the ExpectString exception
    template <typename Pred>
    ParseString &expect(Pred &&, std::string_view);
    
    ///Throw a ExpectChar exception if the front character is not equal to the
    ///supplied character after all characters that satisfy the supplied
    ///predicate have been skipped.
    template <typename Pred>
    ParseString &expectAfter(char, Pred &&);
    ///Throw a ExpectChar exception if the front character is not equal to the
    ///supplied character after all whitespace characters have been skipped
    ParseString &expectAfterWhitespace(char);
    
    ///Advances and returns true if the front character is equal to the supplied
    ///character. Does nothing and returns false otherwise
    bool check(char);
    ///Advances and returns true if the front part of the string is equal to the
    ///supplied string. Does nothing and returns false otherwise
    bool check(const char *, size_t);
    ///Advances and returns true if the front part of the string is equal to the
    ///supplied string. Does nothing and returns false otherwise
    bool check(std::string_view);
    ///Advances and returns true if the front part of the string is equal to the
    ///supplied string. Does nothing and returns false otherwise
    template <size_t SIZE>
    bool check(const char (&)[SIZE]);
    ///Advances and returns true if the supplied predicate returns true for the
    ///first character. Does nothing and returns false otherwise
    template <typename Pred>
    bool check(Pred &&);
    
    ///Interprets the front part of the string as a number. Returns a
    ///positive error code on failure
    template <typename Number>
    std::error_code tryParseNumber(Number &);
    ///Interprets the front part of the string as a number. Throws an
    ///InvalidNumber exception on failure
    template <typename Number>
    ParseString &parseNumber(Number &);
    ///Interprets the front part of the string as a number. Throws an
    ///InvalidNumber exception on failure
    template <typename Number>
    Number parseNumber();
    
    ///Interprets the front part of the string as an enum. Returns the index of
    ///a name that matches or returns the number of names if no name matches.
    size_t tryParseEnum(const std::string_view *, size_t);
    ///Interprest the front part of the string as an enum. Returns an enum with
    ///the matched name or throws a InvalidEnum exception if no name matches.
    template <typename Enum>
    Enum parseEnum(const std::string_view *, size_t);
    
    ///Copies characters from the front part of the string. Advances the number
    ///of characters that were copied.
    size_t copy(char *, size_t);
    ///Copies characters from the front part of the string. Advances the number
    ///of characters that were copied.
    ParseString &copy(std::string &, size_t);
    
    ///Copies characters from the front part of the string while the supplied
    ///predicate returns true. Advances the number of characters that
    ///were copied.
    template <typename Pred>
    size_t copyWhile(char *, size_t, Pred &&);
    ///Copies characters from the front part of the string while the supplied
    ///predicate returns true. Advances the number of characters that
    ///were copied.
    template <typename Pred>
    ParseString &copyWhile(std::string &, Pred &&);
    
    ///Copies characters from the front part of the string until the front is
    ///equal to the supplied character. Advances the number of characters that
    ///were copied.
    size_t copyUntil(char *, size_t, char);
    ///Copies characters from the front part of the string until the front is
    ///equal to the supplied character. Advances the number of characters that
    ///were copied.
    ParseString &copyUntil(std::string &, char);
    
    ///Copies characters from the front part of the string until the supplied
    ///predicate returns true. Advances the number of characters that
    ///were copied.
    template <typename Pred>
    size_t copyUntil(char *, size_t, Pred &&);
    ///Copies characters from the front part of the string until the supplied
    ///predicate returns true. Advances the number of characters that
    ///were copied.
    template <typename Pred>
    ParseString &copyUntil(std::string &, Pred &&);
    
    ///Copies characters from the front part of the string until the front is
    ///whitespace. Advances the number of characters that were copied.
    size_t copyUntilWhitespace(char *, size_t);
    ///Copies characters from the front part of the string until the front is
    ///whitespace. Advances the number of characters that were copied.
    ParseString &copyUntilWhitespace(std::string &);
    
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
