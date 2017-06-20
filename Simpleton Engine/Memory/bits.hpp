//
//  bits.hpp
//  Math
//
//  Created by Indi Kernick on 9/11/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_bits_hpp
#define engine_memory_bits_hpp

#include <cstdint>
#include <cassert>
#include <type_traits>

namespace Memory {
  namespace Bit {
    ///Produces an integer with the I bit on
    template <typename T>
    constexpr auto mask(uint8_t i) -> typename std::enable_if<std::is_integral<T>::value, T>::type {
      return static_cast<T>(1) << i;
    }
    ///Produces an integer with the first N bits on
    template <typename T>
    constexpr auto fillMask(uint8_t n) -> typename std::enable_if<std::is_integral<T>::value, T>::type {
      return (static_cast<T>(1) << n) - 1;
    }
    
    template <typename T>
    inline auto flip(T num, uint8_t i) -> typename std::enable_if<std::is_integral<T>::value, T>::type {
      assert(i < 64);
      return num ^ mask<T>(i);
    }
    
    template <typename T>
    inline auto on(T num, uint8_t i) -> typename std::enable_if<std::is_integral<T>::value, T>::type {
      assert(i < 64);
      return num | mask<T>(i);
    }
    template <typename T>
    inline auto off(T num, uint8_t i) -> typename std::enable_if<std::is_integral<T>::value, T>::type {
      assert(i < 64);
      return num & ~mask<T>(i);
    }
    
    template <typename T>
    inline auto set(T num, uint8_t i, bool value) -> typename std::enable_if<std::is_integral<T>::value, T>::type {
      assert(i < 64);
      return value ? num | mask<T>(i) : num & ~mask<T>(i);
    }
    template <typename T>
    inline auto get(T num, uint8_t i) -> typename std::enable_if<std::is_integral<T>::value, bool>::type {
      assert(i < 64);
      return num & mask<T>(i);
    }
  }
}

#endif
