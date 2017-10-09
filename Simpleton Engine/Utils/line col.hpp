//
//  line col.hpp
//  Game Engine
//
//  Created by Indi Kernick on 25/2/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_line_col_hpp
#define engine_utils_line_col_hpp

#include <cctype>
#include <string>
#include "null check.hpp"

namespace Utils {
  ///Keeps track of lines and columns in text.
  ///Great for writing error messages in parsers
  template <typename Line, typename Col>
  class LineCol {
  public:
    using LineType = Line;
    using ColType = Col;
    
    static constexpr ColType  SIZE_OF_TAB = 8;
    static constexpr LineType FIRST_LINE  = 1;
    static constexpr ColType  FIRST_COL   = 1;
    
    // 3.9 lines and 18.4 columns doesn't make any sense!
    static_assert(std::is_integral<LineType>::value, "Type of line must be an integer");
    static_assert(std::is_integral<ColType>::value, "Type of column must be an integer");
  
    LineCol()
      : line(FIRST_LINE), col(FIRST_COL) {}
    LineCol(const LineType line, const ColType col)
      : line(line), col(col) {
      if (line < FIRST_LINE || col < FIRST_COL) {
        throw std::out_of_range("Line or column too small");
      }
    }
    
    /*@TODO uncomment
    
    template <typename LineT, typename ColT>
    LineCol(const LineT line, const ColT col) -> LineCol<LineT, ColT>
      : line(line), col(col) {
      if (line < FIRST_LINE || col < FIRST_COL) {
        throw std::out_of_range("Line or column too small");
      }
    }*/
    
    ///Move line and col according to the char
    void putChar(const char c) {
      switch (c) {
        case '\t':
          //assumes that ColType is an integer
          col = (col + SIZE_OF_TAB - 1) / SIZE_OF_TAB * SIZE_OF_TAB;
          break;
        case '\n':
          line++;
          col = FIRST_COL;
          break;
        case '\v':
        case '\f':
          line++;
          break;
        case '\r':
          col = FIRST_COL;
          break;
        case '\b':
          if (col != FIRST_COL) {
            col--;
          }
          break;
        
        default:
          if (std::isprint(c)) {
            col++;
          }
      }
    }
    ///Call putChar(char) for the first n chars in the string
    void putString(const char *str, size_t size) {
      throwIfNull(str);
      while (size) {
        putChar(*str);
        ++str;
        --size;
      }
    }
    ///Sets line to FIRST_LINE and col to FIRST_COL
    void reset() {
      line = FIRST_LINE;
      col = FIRST_COL;
    }
    
    ///Sets line and col
    void moveTo(const LineType newLine, const ColType newCol) {
      line = newLine;
      col = newCol;
    }
    ///Adds line and col
    void moveBy(const LineType deltaLine, const ColType deltaCol) {
      line += deltaLine;
      col += deltaCol;
    }
    
    LineCol &operator+=(const LineCol other) {
      line += other.line;
      col += other.col;
      return *this;
    }
    LineCol &operator-=(const LineCol other) {
      line -= other.line;
      col -= other.col;
      return *this;
    }
    LineCol operator+(const LineCol other) const {
      return {
        line + other.line,
        col + other.col
      };
    }
    LineCol operator-(const LineCol other) const {
      return {
        line - other.line,
        col - other.col
      };
    }
    
    LineType getLine() const {
      return line;
    }
    ColType getCol() const {
      return col;
    }
    
    const char *asStr() const {
      //The largest 64 bit unsigned integer value is 18446744073709551615 which is
      //20 characters. 20 + ':' + 20 + '\0' = 42 = the answer to the ultimate question
      static char str[42];
      
      std::string numString = std::to_string(line);
      std::copy(numString.cbegin(), numString.cend(), str);
      size_t lineStrSize = numString.size();
      str[lineStrSize] = ':';
      numString = std::to_string(col);
      std::copy(numString.cbegin(), numString.cend(), str + lineStrSize + 1);
      str[lineStrSize + 1 + numString.size()] = 0;
      
      /*
      @TODO uncomment
      
      const auto [end, error] = std::to_chars(str, str + 42, line);
      if (error) {
        throw std::runtime_error(error.message());
      }
      *end = ':';
      const auto [end1, error1] = std::to_chars(end + 1, str + 42, col);
      if (error1) {
        throw std::runtime_error(error1.message());
      }*/
      
      return str;
    }
    
  private:
    LineType line;
    ColType col;
  };
}

#endif
