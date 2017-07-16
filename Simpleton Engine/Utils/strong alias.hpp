//
//  strong alias.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 16/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_strong_alias_hpp
#define engine_utils_strong_alias_hpp

#include <type_traits>

namespace Utils {
  template <typename T>
  struct StrongAlias {
    using underlying_type = T;
    
    StrongAlias() = default;
    StrongAlias(const StrongAlias &) = default;
    StrongAlias(StrongAlias &&) = default;
    ~StrongAlias() = default;
    StrongAlias &operator=(const StrongAlias &) = default;
    StrongAlias &operator=(StrongAlias &&) = default;
    
    explicit StrongAlias(const T val)
      : val(val) {}
    
    explicit operator T() const {
      return val;
    }
    
    decltype(!std::declval<const T>()) operator!() const {
      return !val;
    }
    decltype(~std::declval<const T>()) operator~() const {
      return ~val;
    }
    
    decltype(++std::declval<T &>()) operator++() {
      return ++val;
    }
    decltype(--std::declval<T &>()) operator--() {
      return --val;
    }
    
    decltype(std::declval<T &>()++) operator++(int) {
      return val++;
    }
    decltype(std::declval<T &>()--) operator--(int) {
      return val--;
    }
    
    T val;
  };
}

#define BIN_OP(OP)                                                              \
template <typename T>                                                           \
decltype(std::declval<const T>() OP std::declval<const T>())                    \
operator OP(                                                                    \
  const Utils::StrongAlias<T> left,                                             \
  const Utils::StrongAlias<T> right                                             \
) {                                                                             \
  return left.val OP right.val;                                                 \
}                                                                               \
template <typename Left, typename Right>                                        \
decltype(std::declval<const Left>() OP std::declval<const Right>())             \
operator OP(const Utils::StrongAlias<Left> left, const Right right) {           \
  return left.val OP right;                                                     \
}                                                                               \
template <typename Left, typename Right>                                        \
decltype(std::declval<const Left>() OP std::declval<const Right>())             \
operator OP(const Left left, const Utils::StrongAlias<Right> right) {           \
  return left OP right.val;                                                     \
}

BIN_OP(==)
BIN_OP(!=)
BIN_OP(<)
BIN_OP(>)
BIN_OP(<=)
BIN_OP(>=)
BIN_OP(&&)
BIN_OP(||)
BIN_OP(+)
BIN_OP(-)
BIN_OP(*)
BIN_OP(/)
BIN_OP(%)
BIN_OP(&)
BIN_OP(|)
BIN_OP(^)
BIN_OP(<<)
BIN_OP(>>)

#undef BIN_OP

#define ASSIGN_OP(OP)                                                           \
template <typename T>                                                           \
decltype(std::declval<T &>() OP std::declval<const T>())                        \
operator OP(Utils::StrongAlias<T> &left, const Utils::StrongAlias<T> right) {   \
  return left.val OP right.val;                                                 \
}                                                                               \
template <typename Left, typename Right>                                        \
decltype(std::declval<Left &>() OP std::declval<const Right>())                 \
operator OP(Utils::StrongAlias<Left> &left, const Right right) {                \
  return left.val OP right;                                                     \
}                                                                               \
template <typename Left, typename Right>                                        \
decltype(std::declval<Left &>() OP std::declval<const Right>())                 \
operator OP(Left &left, const Utils::StrongAlias<Right> right) {                \
  return left OP right.val;                                                     \
}

ASSIGN_OP(+=)
ASSIGN_OP(-=)
ASSIGN_OP(*=)
ASSIGN_OP(/=)
ASSIGN_OP(%=)
ASSIGN_OP(&=)
ASSIGN_OP(|=)
ASSIGN_OP(^=)
ASSIGN_OP(<<=)
ASSIGN_OP(>>=)

#undef ASSIGN_OP

#endif
