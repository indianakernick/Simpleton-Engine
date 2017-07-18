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
    using value_type = T;
    
    StrongAlias() = default;
    StrongAlias(const StrongAlias &) = default;
    StrongAlias(StrongAlias &&) = default;
    ~StrongAlias() = default;
    StrongAlias &operator=(const StrongAlias &) = default;
    StrongAlias &operator=(StrongAlias &&) = default;
    
    explicit StrongAlias(const T val)
      : val(val) {}
    
    template <typename U>
    explicit StrongAlias(const U val)
      : val(static_cast<T>(val)) {}
    
    explicit operator T() const {
      return val;
    }
    
    template <typename U>
    explicit operator U() const {
      return static_cast<U>(val);
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
    
    template <typename U>
    decltype(std::declval<const T>()[std::declval<U>()]) operator[](const U i) const {
      return val[i];
    }
    template <typename U>
    decltype(std::declval<T>()[std::declval<U>()]) operator[](const U i) {
      return val[i];
    }
    
    decltype(*std::declval<const T>()) operator*() const {
      return *val;
    }
    decltype(*std::declval<T>()) operator*() {
      return *val;
    }
    
    //works for raw pointers and smart pointers
    std::conditional_t<
      std::is_pointer<const T>::value,
      const T,
      decltype(std::declval<const T>().operator->())
    >
    operator->() const {
      if constexpr (std::is_pointer<const T>::value) {
        return val;
      } else {
        return val.operator->();
      }
    }
    std::conditional_t<
      std::is_pointer<T>::value,
      T,
      decltype(std::declval<T>().operator->())
    >
    operator->() {
      if constexpr (std::is_pointer<T>::value) {
        return val;
      } else {
        return val.operator->();
      }
    }
    
    T val;
  };
}

template <typename T, typename Ret>
struct GetRetType {
  using type = Ret;
};

template <typename T>
struct GetRetType<T, T> {
  using type = Utils::StrongAlias<T>;
};

template <typename T>
struct GetRetType<T, T &> {
  using type = Utils::StrongAlias<T> &;
};

template <typename T, typename Ret>
using RetType = typename GetRetType<T, Ret>::type;

#define RET_TYPE(LEFT, RIGHT, RET, OP)                                          \
RetType<RET, decltype(std::declval<LEFT>() OP std::declval<RIGHT>())>

#define BIN_OP(OP)                                                              \
template <typename T>                                                           \
RET_TYPE(T, T, T, OP)                                                           \
operator OP(                                                                    \
  const Utils::StrongAlias<T> left,                                             \
  const Utils::StrongAlias<T> right                                             \
) {                                                                             \
  return RET_TYPE(T, T, T, OP)(left.val OP right.val);                          \
}                                                                               \
template <typename Left, typename Right>                                        \
RET_TYPE(Left, Right, Left, OP)                                                 \
operator OP(const Utils::StrongAlias<Left> left, const Right right) {           \
  return RET_TYPE(Left, Right, Left, OP)(left.val OP right);                    \
}                                                                               \
template <typename Left, typename Right>                                        \
RET_TYPE(Left, Right, Right, OP)                                                \
operator OP(const Left left, const Utils::StrongAlias<Right> right) {           \
  return RET_TYPE(Left, Right, Right, OP)(left OP right.val);                   \
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
RET_TYPE(T &, T, T, OP)                                                         \
operator OP(Utils::StrongAlias<T> &left, const Utils::StrongAlias<T> right) {   \
  return RET_TYPE(T &, T, T, OP)(left.val OP right.val);                        \
}                                                                               \
template <typename Left, typename Right>                                        \
RET_TYPE(Left &, Right, Left, OP)                                               \
operator OP(Utils::StrongAlias<Left> &left, const Right right) {                \
  return RET_TYPE(Left &, Right, Left, OP)(left.val OP right);                  \
}                                                                               \
template <typename Left, typename Right>                                        \
RET_TYPE(Left &, Right, Right, OP)                                              \
operator OP(Left &left, const Utils::StrongAlias<Right> right) {                \
  return RET_TYPE(Left &, Right, Right, OP)(left OP right.val);                 \
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
#undef RET_TYPE

#endif
