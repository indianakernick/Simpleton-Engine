//
//  combine.hpp
//  Game Engine
//
//  Created by Indi Kernick on 4/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_combine_hpp
#define engine_utils_combine_hpp

#include <functional>
#include "int least.hpp"
#include "variadic type traits.hpp"
#include "construct from tuple.hpp"

namespace Utils {
  template <typename First>
  constexpr std::enable_if_t<
    std::is_standard_layout<First>::value,
    uint_fit_t<First>
  >
  combine(const First first) {
    return static_cast<uint_fit_t<First>>(first);
  }

  template <typename First, typename ...Rest>
  constexpr std::enable_if_t<
    all<std::is_standard_layout, First, Rest...>,
    uint_fit_t<First, Rest...>
  >
  combine(const First first, const Rest... rest) {
    return (
      static_cast<uint_fit_t<First, Rest...>>(first) << (
        (sizeof(Rest) + ...) * 8
      )
    ) | combine(rest...);
  }
  
  //Int cannot be the last template parameter because Rest is a parameter pack.
  //Overloading the function provides a better inferface
  //Concepts TS solves this problem because I can use the name of a concept
  //instead of creating a template
  
  #define DECOMPOSE(Int)                                                        \
  template <typename First>                                                     \
  constexpr std::enable_if_t<                                                   \
    std::is_standard_layout<First>::value &&                                    \
    sizeof(First) <= sizeof(Int),                                               \
    std::tuple<First>                                                           \
  >                                                                             \
  decompose(const Int set) {                                                    \
    constexpr std::make_unsigned_t<Int> MASK = uint_fit_t<First>(-1);           \
    return std::tuple<First>(set & MASK);                                       \
  }                                                                             \
                                                                                \
  template <typename First, typename ...Rest>                                   \
  constexpr std::enable_if_t<                                                   \
    all<std::is_standard_layout, First, Rest...> &&                             \
    (sizeof...(Rest) > 0) &&                                                    \
    sizeof(First) + (sizeof(Rest) + ...) <= sizeof(Int),                        \
    std::tuple<First, Rest...>                                                  \
  >                                                                             \
  decompose(const Int set) {                                                    \
    constexpr size_t BIT_SIZE = (sizeof(Rest) + ...) * 8;                       \
    constexpr std::make_unsigned_t<Int> MASK = uint_fit_t<First>(-1);           \
    return std::tuple_cat(                                                      \
      std::tuple<First>((set >> BIT_SIZE) & MASK),                              \
      decompose<Rest...>(set)                                                   \
    );                                                                          \
  }                                                                             \
                                                                                \
  template <typename Type, typename ...Args>                                    \
  constexpr Type decomposeAndConstruct(const Int set) {                         \
    return constructFromTuple<Type>(decompose<Args...>(set));                   \
  }
  
  DECOMPOSE(int8_t)
  DECOMPOSE(uint8_t)
  DECOMPOSE(int16_t)
  DECOMPOSE(uint16_t)
  DECOMPOSE(int32_t)
  DECOMPOSE(uint32_t)
  DECOMPOSE(int64_t)
  DECOMPOSE(uint64_t)
  #if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ > __SIZEOF_INT64__
  DECOMPOSE(__int128)
  DECOMPOSE(unsigned __int128)
  #endif
  
  #undef DECOMPOSE

  constexpr uint32_t boolCombine(const bool first) {
    return static_cast<uint32_t>(first);
  }

  template <typename ...Rest>
  constexpr std::enable_if_t<allSameAs<bool, Rest...>, uint32_t>
  boolCombine(const bool first, const Rest... rest) {
    return (static_cast<uint32_t>(first) << sizeof...(Rest)) | boolCombine(rest...);
  }

  //based on boost::hash_combine

  template <typename First>
  size_t hashCombine(First first) {
    const std::hash<First> hasher;
    return hasher(first);
  }

  template <typename First, typename ...Rest>
  size_t hashCombine(First first, Rest... rest) {
    const std::hash<First> hasher;
    const size_t firstHash = hasher(first);
    return firstHash ^ (hashCombine(rest...) + 0x9e3779b9 + (firstHash << 6) + (firstHash >> 2));
  }
}

#endif
