//
//  collision categories.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 29/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_box2d_collision_categories_hpp
#define engine_box2d_collision_categories_hpp

#ifndef CATEGORIES
/*
An example of a possible definition of CATEGORIES is this

#define CATEGORIES                                                              \
  CAT(Object)                                                                   \
  CAT(PassThrough)                                                              \
  CAT(Player)                                                                   \
  LAST_CAT(Enemy)
*/
#error "Must #define CATEGORIES before #including this header"
#else

#include <string>
#include <cstdint>
#include "../Utils/type list.hpp"

namespace B2 {
  using CategoryBit = uint16_t;

  namespace Cat {
    #define CAT(NAME) class NAME {};
    #define LAST_CAT(NAME) CAT(NAME)
    CATEGORIES
    #undef LAST_CAT
    #undef CAT
  }
  
  using Categories = Utils::TypeList<
    #define CAT(NAME) Cat::NAME,
    #define LAST_CAT(NAME) Cat::NAME
    CATEGORIES
    #undef LAST_CAT
    #undef CAT
  >;
  
  static_assert(Utils::listSize<Categories> <= 16);
  
  inline CategoryBit getCategoryBit(const std::string &categoryName) {
    try {
      return Utils::getValueByName<CategoryBit, Categories>(
        "B2::Cat::" + categoryName,
        [] (auto t) {
          return 1 << Utils::indexOf<Categories, UTILS_TYPE(t)>;
        }
      );
    } catch (Utils::TypeNotFound &) {
      throw std::runtime_error("Invalid category name");
    }
  }
}

#endif // CATEGORIES

#endif
