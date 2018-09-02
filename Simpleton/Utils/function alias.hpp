//
//  function alias.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/5/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_function_alias_hpp
#define engine_utils_function_alias_hpp

#include <utility>

/// Create a function alias using a function pointer
#define FUN_ALIAS_PTR(NEW_NAME, ...)                                            \
  constexpr auto NEW_NAME = __VA_ARGS__;

/// Create a function alias with specifiers
#define FUN_ALIAS_SPEC(SPECS, NEW_NAME, ...)                                    \
  template <typename... Args>                                                   \
  SPECS decltype(auto) NEW_NAME(Args &&... args)                                \
    noexcept(noexcept(__VA_ARGS__(std::forward<Args>(args)...))) {              \
    return __VA_ARGS__(std::forward<Args>(args)...);                            \
  }

/// Create a function alias
#define FUN_ALIAS(NEW_NAME, ...)                                                \
  FUN_ALIAS_SPEC(inline, NEW_NAME, __VA_ARGS__)

/// Create a function alias and forward template type parameters with specifiers
#define FUN_ALIAS_TMP_TYPE_SPEC(SPECS, NEW_NAME, ...)                           \
  template <typename... Params, typename... Args>                               \
  SPECS decltype(auto) NEW_NAME(Args &&... args)                                \
    noexcept(noexcept(__VA_ARGS__<Params...>(std::forward<Args>(args)...))) {   \
    return __VA_ARGS__<Params...>(std::forward<Args>(args)...);                 \
  }

/// Create a function alias and forward template value parameters with specifiers
#define FUN_ALIAS_TMP_VAL_SPEC(SPECS, NEW_NAME, ...)                            \
  template <auto... Params, typename... Args>                                   \
  SPECS decltype(auto) NEW_NAME(Args &&... args)                                \
    noexcept(noexcept(__VA_ARGS__<Params...>(std::forward<Args>(args)...))) {   \
    return __VA_ARGS__<Params...>(std::forward<Args>(args)...);                 \
  }

/// Create a function alias using a lambda with specifiers
#define FUN_ALIAS_LAM_SPEC(SPECS, NEW_NAME, ...)                                            \
  SPECS auto NEW_NAME = [] (auto &&... args)                                    \
    noexcept(noexcept(__VA_ARGS__(std::forward<decltype(args)>(args)...)))      \
    -> decltype(auto) {                                                         \
    return __VA_ARGS__(std::forward<decltype(args)>(args)...);                  \
  };

/// Create a function alias using a lambda
#define FUN_ALIAS_LAM(NEW_NAME, ...)                                            \
  FUN_ALIAS_LAM_SPEC(constexpr, NEW_NAME, __VA_ARGS__)

#endif
