//
//  round.hpp
//  Game Engine
//
//  Created by Indi Kernick on 10/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_math_round_hpp
#define engine_math_round_hpp

#include "pow.hpp"

namespace Math {
  ///Ceil num to the nearest power of 2
  constexpr uint64_t ceilToPowerOf2(uint64_t num) {
    assert(num != 0);
    return (1 << (64 - __builtin_clzll(num - 1))) - (num == 1);
  }
  
  ///Floor num to the nearest power of 2
  constexpr uint64_t floorToPowerOf2(uint64_t num) {
    assert(num != 0);
    return (1 << (63 - __builtin_clzll(num)));
  }
  
  ///Ceil num to the nearest power of base
  constexpr uint64_t ceilToPower(uint64_t base, uint64_t num) {
    return pow(base, logCeil(base, num));
  }
  
  ///Floor num to the nearest power of base
  constexpr uint64_t floorToPower(uint64_t base, uint64_t num) {
    return pow(base, log(base, num));
  }
  
  ///Ceil num to the nearest multiple of factor
  constexpr uint64_t ceilToMultiple(uint64_t factor, uint64_t num) {
    return (num + factor - 1) / factor * factor;
  }
  
  ///Floor num to the nearest multiple of factor
  constexpr uint64_t floorToMultiple(uint64_t factor, uint64_t num) {
    return num / factor * factor;
  }
  
  ///Compute the ceil of the division of num and den
  template <typename NUM, typename DEN>
  constexpr std::enable_if_t<std::is_unsigned<NUM>::value &&
                             std::is_unsigned<DEN>::value,
                             std::common_type_t<NUM, DEN>>
  divCeil(NUM num, DEN den) {
    return (num + den - 1) / den;
  }
  
  //I found the following on stackoverflow
  //http://stackoverflow.com/a/17005764
  
  ///Compute the ceil of the division of num and den
  template <typename NUM, typename DEN>
  constexpr std::enable_if_t<std::is_signed<NUM>::value ||
                             std::is_signed<DEN>::value,
                             std::common_type_t<NUM, DEN>>
  divCeil(NUM num, DEN den) {
    return num / den + (((num < 0) ^ (den > 0)) && (num % den));
  }
  
  ///Compute the round of the division of num and den
  template <typename NUM, typename DEN>
  constexpr std::enable_if_t<std::is_unsigned<NUM>::value &&
                             std::is_unsigned<DEN>::value,
                             std::common_type_t<NUM, DEN>>
  divRound(NUM num, DEN den) {
    return (num + den / 2) / den;
  }
  
  //http://stackoverflow.com/a/18067292
  
  ///Compute the round of the division of num and den
  template <typename NUM, typename DEN>
  constexpr std::enable_if_t<std::is_signed<NUM>::value ||
                             std::is_signed<DEN>::value,
                             std::common_type_t<NUM, DEN>>
  divRound(NUM num, DEN den) {
    return (num < 0) ^ (den < 0) ? (num - den / 2) / den
                                 : (num + den / 2) / den;
  }
}

#endif
