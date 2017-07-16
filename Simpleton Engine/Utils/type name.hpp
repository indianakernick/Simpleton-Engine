//
//  type name.hpp
//  Game Engine
//
//  Created by Indi Kernick on 18/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_type_name_hpp
#define engine_utils_type_name_hpp

#include <experimental/string_view>

namespace Utils {
  template <typename T>
  constexpr std::experimental::string_view typeName() {
    //pretty function outputs
    //std::experimental::string_view typeNameView() [T = int]
    const char *data = __PRETTY_FUNCTION__;
    while (*data != '[') {
      data++;
    }
    //trimming "[T = "
    data += 5;
    
    //subtracting 1 from the actual size of the string trims ']'
    size_t size = std::numeric_limits<size_t>::max();
    {
      const char *c = data;
      while (*c) {
        size++;
        c++;
      }
    }
    
    return {data, size};
  }
  
  template <typename T>
  constexpr size_t typeHash() {
    //djb2
    
    constexpr std::experimental::string_view name = typeName<T>();
    
    const char *data = name.data();
    const size_t size = name.size();
    size_t hash = 5381;
    
    for (size_t i = 0; i != size; i++) {
      hash = ((hash << 5) + hash) + data[i];
    }

    return hash;
  }
}

#endif
