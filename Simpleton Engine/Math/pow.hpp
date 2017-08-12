//
//  pow.hpp
//  Game Engine
//
//  Created by Indi Kernick on 6/11/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_math_pow_hpp
#define engine_math_pow_hpp

#include <type_traits>
#include <cassert>

namespace Math {
  template <typename Number>
  constexpr std::enable_if_t<
    std::is_arithmetic<Number>::value,
    Number
  >
  square(const Number n) {
    return n * n;
  }
  
  template <typename Number>
  constexpr std::enable_if_t<
    std::is_arithmetic<Number>::value,
    Number
  >
  cube(const Number n) {
    return n * n * n;
  }

  //24% faster than std::pow for integers

  template <typename Number, typename Exponent>
  constexpr std::enable_if_t<
    std::is_arithmetic<Number>::value &&
    std::is_unsigned<Exponent>::value,
    Number
  >
  pow(const Number num, Exponent exp) {
    Number out = 1;
    while (exp--) {
      out *= num;
    }
    return out;
  }
  
  template <typename Number, typename Exponent>
  constexpr std::enable_if_t<
    std::is_arithmetic<Number>::value &&
    std::is_signed<Exponent>::value,
    Number
  >
  pow(const Number num, const Exponent exp) {
    if (exp < 0) {
      return 1 / pow(num, static_cast<std::make_unsigned_t<Exponent>>(-exp));
    } else {
      return pow(num, static_cast<std::make_unsigned_t<Exponent>>(exp));
    }
  }
  
  //1314% faster than std::log2
  
  ///Compute the floor of the base 2 logarithm of a number
  constexpr unsigned long long log2(const unsigned long long num) {
    assert(num != 0);
  
    return (sizeof(unsigned long long) * 8 - 1) - __builtin_clzll(num);
  }
  
  ///Compute the floor of the base 2 logarithm of a number
  constexpr unsigned long log2(const unsigned long num) {
    assert(num != 0);
  
    return (sizeof(unsigned long) * 8 - 1) - __builtin_clzl(num);
  }
  
  ///Compute the floor of the base 2 logarithm of a number
  constexpr unsigned log2(const unsigned num) {
    assert(num != 0);
  
    return (sizeof(unsigned) * 8 - 1) - __builtin_clz(num);
  }
  
  ///Compute the floor of the base 2 logarithm of a number
  constexpr unsigned short log2(const unsigned short num) {
    return log2(static_cast<unsigned>(num));
  }
  
  ///Compute the floor of the base 2 logarithm of a number
  constexpr unsigned char log2(const unsigned char num) {
    return log2(static_cast<unsigned>(num));
  }
  
  //272.3% faster than std::ceil(std::log2)
  
  ///Compute the ceil of the base 2 logarithm of a number
  constexpr unsigned long long log2Ceil(const unsigned long long num) {
    assert(num != 0);
    
    const unsigned long long leading = __builtin_clzll(num);
    constexpr unsigned long long bits = sizeof(unsigned long long) * 8;
    return bits - leading - (leading + __builtin_ctzll(num) == bits - 1);
  }
  
  ///Compute the ceil of the base 2 logarithm of a number
  constexpr unsigned long log2Ceil(const unsigned long num) {
    assert(num != 0);
    
    const unsigned long leading = __builtin_clzl(num);
    constexpr unsigned long bits = sizeof(unsigned long) * 8;
    return bits - leading - (leading + __builtin_ctzl(num) == bits - 1);
  }
  
  ///Compute the ceil of the base 2 logarithm of a number
  constexpr unsigned log2Ceil(const unsigned num) {
    assert(num != 0);
    
    const unsigned leading = __builtin_clz(num);
    constexpr unsigned bits = sizeof(unsigned) * 8;
    return bits - leading - (leading + __builtin_ctz(num) == bits - 1);
  }
  
  ///Compute the ceil of the base 2 logarithm of a number
  constexpr unsigned short log2Ceil(const unsigned short num) {
    return log2Ceil(static_cast<unsigned>(num));
  }
  
  ///Compute the ceil of the base 2 logarithm of a number
  constexpr unsigned char log2Ceil(const unsigned char num) {
    return log2Ceil(static_cast<unsigned>(num));
  }
  
  ///Compute the floor of the logarithm of a number
  template <typename UnsignedInt>
  constexpr std::enable_if_t<
    std::is_unsigned<UnsignedInt>::value,
    UnsignedInt
  >
  log(const UnsignedInt base, UnsignedInt num) {
    assert(base > 1);
    assert(num != 0);
    
    UnsignedInt count = 0;
    while (num /= base) {
      ++count;
    }
    return count;
  }
  
  ///Compute the ceil of the logarithm of a number
  template <typename UnsignedInt>
  constexpr std::enable_if_t<
    std::is_unsigned<UnsignedInt>::value,
    UnsignedInt
  >
  logCeil(const UnsignedInt base, UnsignedInt num) {
    assert(base > 1);
    assert(num != 0);
    
    UnsignedInt count = 0;
    bool up = false;
    while (UnsignedInt numDivBase = num / base) {
      up = up || (num % base);
      ++count;
      num = numDivBase;
    }
    return count + up;
  }
  
  ///Is num a power of 2?
  template <typename UnsignedInt>
  constexpr std::enable_if_t<
    std::is_unsigned<UnsignedInt>::value,
    bool
  >
  isPowerOf2(const UnsignedInt num) {
    assert(num != 0);
    
    return (num & (num - 1)) == 0;
  }
  
  ///Is num a power of base?
  template <typename UnsignedInt>
  constexpr std::enable_if_t<
    std::is_unsigned<UnsignedInt>::value,
    bool
  >
  isPower(const UnsignedInt base, UnsignedInt num) {
    assert(base > 1);
    assert(num != 0);
    
    while (num % base == 0) {
      num /= base;
    }
    return num == 1;
  }
}

#endif
