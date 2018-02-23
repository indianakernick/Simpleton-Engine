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
    
    std::string_view view() const;
    bool empty() const;
    
    void write(const char *);
    void write(const char *, size_t);
    void write(std::string_view);
    
    template <typename Number>
    void writeNumber(Number);
    
    template <typename Enum>
    void writeEnum(Enum, const std::string_view *);
    
    void open(const std::string &, const std::string &);
    void close();
    void closeAll();
    
  private:
    std::string string;
    std::stack<std::string> closingStrings;
  };
}

#include "compose string.inl"

#endif
