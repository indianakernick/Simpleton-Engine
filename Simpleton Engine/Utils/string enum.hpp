//
//  string enum.hpp
//  Game Engine
//
//  Created by Indi Kernick on 20/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_string_enum_hpp
#define engine_utils_string_enum_hpp

#include "int least.hpp"
#include <stdexcept>
#include <experimental/string_view>

namespace Utils {
  template <typename EnumType, size_t SIZE, const std::experimental::string_view (&STRINGS)[SIZE]>
  class StringEnum {
  private:
    using IntType = uint_fit_t<EnumType>;
    static_assert(std::numeric_limits<IntType>::max() >= SIZE);

  public:
    using String = std::experimental::string_view;

    static EnumType strToEnum(const String string) {
      IntType value = 0;
      while (value < SIZE && STRINGS[value] != string) {
        value++;
      }
      if (value == SIZE) {
        throw std::range_error(std::string("Invalid enum string \"") + string.data() + "\"");
      }
      return static_cast<EnumType>(value);
    }

    static String enumToStr(const EnumType enumValue) {
      const IntType intValue = static_cast<IntType>(enumValue);
      if (intValue < 0 || intValue >= SIZE) {
        throw std::range_error("Invalid enum value");
      }
      return STRINGS[intValue];
    }
  };
}

#endif
