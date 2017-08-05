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
    size_t hash = 5381;
    
    for (auto c = name.begin(); c != name.end(); ++c) {
      hash = ((hash << 5) + hash) + *c;
    }

    return hash;
  }
  
  template <typename T0, typename T1>
  constexpr bool typeLess() {
    constexpr std::experimental::string_view name0 = typeName<T0>();
    constexpr std::experimental::string_view name1 = typeName<T1>();
    constexpr size_t minSize = name0.size() < name1.size()
                             ? name0.size()
                             : name1.size();
    
    for (size_t i = 0; i != minSize; ++i) {
      if (name0[i] != name1[i]) {
        return name0[i] < name1[i];
      }
    }
    
    return name0.size() < name1.size();
  }
}

#endif
