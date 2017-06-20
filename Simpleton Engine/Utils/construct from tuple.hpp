//
//  construct from tuple.hpp
//  Game Engine
//
//  Created by Indi Kernick on 12/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_construct_from_tuple_hpp
#define engine_utils_construct_from_tuple_hpp

#include <tuple>

namespace Utils {
  template <typename Type, typename Tuple, size_t ...Is>
  constexpr Type constructFromTuple(Tuple &&tuple, std::index_sequence<Is...>) {
    return {std::get<Is>(std::forward<Tuple>(tuple))...};
  }
  
  template <typename Type, typename Tuple>
  constexpr Type constructFromTuple(Tuple &&tuple) {
    return constructFromTuple<Type>(
      std::forward<Tuple>(tuple),
      std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>()
    );
  }
}

#endif
