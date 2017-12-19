//
//  type name.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 18/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_type_name_hpp
#define engine_utils_type_name_hpp

#include <string_view>

namespace Utils {
  template <typename T>
  constexpr std::string_view typeName() {
    //pretty function outputs
    //std::string_view typeName() [T = int]
    std::string_view name = __PRETTY_FUNCTION__;
    name.remove_prefix(name.find('['));
    //trimming "[T = "
    name.remove_prefix(5);
    //trimming "]"
    name.remove_suffix(1);
    return name;
  }
  
  template <typename T>
  constexpr size_t typeHash() {
    //djb2
    constexpr std::string_view name = typeName<T>();
    size_t hash = 5381;
    
    for (auto c = name.begin(); c != name.end(); ++c) {
      hash = ((hash << 5) + hash) + *c;
    }

    return hash;
  }
  
  template <typename T0, typename T1>
  constexpr bool typeLess() {
    return typeName<T0>() < typeName<T1>();
  }
}

#endif
