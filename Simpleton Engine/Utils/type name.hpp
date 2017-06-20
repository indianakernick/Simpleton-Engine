//
//  type name.hpp
//  Game Engine
//
//  Created by Indi Kernick on 18/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_type_name_hpp
#define engine_utils_type_name_hpp

#include <string>

namespace Utils {
  ///Demangles the name returned by std::type_info::name()
  std::string demangle(const char *);

  ///Get the formatted name for a type
  template <typename T>
  std::string typeName() {
    return demangle(typeid(T).name());
  }
}

#endif
