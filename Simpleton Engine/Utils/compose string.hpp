//
//  compose string.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_compose_string_hpp
#define engine_utils_compose_string_hpp

#include <stack>
#include <string>
#include <string_view>

namespace Utils {
  class ComposeString {
  public:
    explicit ComposeString(size_t = 4096);
    
    char *begin();
    char *curr();
    char *end();
    std::string_view view() const;
    size_t freeSpace() const;
    
    void reserve(size_t);
    void reserveToFit(size_t);
    void addSize(size_t);
    bool empty() const;
    
    void write(char);
    void write(const char *, size_t);
    void write(std::string_view);
    
    template <typename Number>
    void writeNumber(Number);
    template <typename Number>
    void writeNumberLil(Number);
    template <typename Number>
    void writeNumberBig(Number);
    
    template <typename Number>
    void writeNumbersLil(const Number *, size_t);
    template <typename Number>
    void writeNumbersBig(const Number *, size_t);
    
    template <typename Enum>
    void writeEnum(Enum, const std::string_view *);
    
    void open(const std::string &, const std::string &);
    void close();
    void closeAll();
    
  private:
    std::unique_ptr<char []> string;
    size_t size;
    size_t capacity;
    std::stack<std::string> closingStrings;
    
    void setCapacity(size_t);
  };
}

#include "compose string.inl"

#endif
