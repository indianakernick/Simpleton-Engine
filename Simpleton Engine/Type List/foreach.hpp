//
//  foreach.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 2/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_type_list_foreach_hpp
#define engine_type_list_foreach_hpp

#include "type.hpp"

namespace List {
  namespace detail {
    template <typename T>
    struct TypeObj {
      using type = T;
    };
    
    #define LIST_TYPE(OBJ) typename decltype(OBJ)::type
    
    template <typename List>
    struct ForEachI;
    
    template <typename... Types>
    struct ForEachI<Type<Types...>> {
      template <typename Function>
      static constexpr void iter(Function &&function) {
        (function(TypeObj<Types>()), ...);
      }
    };
  }
  
  /// Call the given template function (or lambda) for each type in the list.
  /// The function is called with an instance of List::detail::TypeObj which has
  /// a type member
  template <typename List, typename Function>
  constexpr void forEach(Function &&func) {
    detail::ForEachI<List>::iter(func);
  }
}

#endif
