//
//  generic raii.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 23/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_generic_raii_hpp
#define engine_utils_generic_raii_hpp

#include <utility>

// Same interface as std::unique_ptr (the best class in the entire STL)

#define UTILS_RAII_CLASS(CLASS, TYPE, MEMBER, DELETE)                           \
  CLASS() noexcept                                                              \
    : MEMBER() {}                                                               \
  CLASS(std::nullptr_t) noexcept                                                \
    : MEMBER() {}                                                               \
  explicit CLASS(const TYPE MEMBER) noexcept                                    \
    : MEMBER(MEMBER) {}                                                         \
  CLASS(CLASS &&other) noexcept                                                 \
    : MEMBER(other.release()) {}                                                \
  CLASS &operator=(CLASS &&other) noexcept {                                    \
    reset(other.release());                                                     \
    return *this;                                                               \
  }                                                                             \
  CLASS &operator=(std::nullptr_t) noexcept {                                   \
    reset();                                                                    \
    return *this;                                                               \
  }                                                                             \
  ~CLASS() {                                                                    \
    DELETE(MEMBER);                                                             \
  }                                                                             \
                                                                                \
  TYPE release() noexcept {                                                     \
    return std::exchange(MEMBER, TYPE());                                       \
  }                                                                             \
  void reset(const TYPE new##MEMBER = TYPE()) noexcept {                        \
    DELETE(MEMBER);                                                             \
    MEMBER = new##MEMBER;                                                       \
  }                                                                             \
  void swap(CLASS &other) noexcept {                                            \
    std::swap(MEMBER, other.MEMBER);                                            \
  }                                                                             \
                                                                                \
  TYPE get() const noexcept {                                                   \
    return MEMBER;                                                              \
  }                                                                             \
  explicit operator bool() const noexcept {                                     \
    return MEMBER != TYPE();                                                    \
  }                                                                             \
                                                                                \
  bool operator==(const CLASS &other) const noexcept {                          \
    return MEMBER == other.MEMBER;                                              \
  }                                                                             \
  bool operator!=(const CLASS &other) const noexcept {                          \
    return MEMBER != other.MEMBER;                                              \
  }                                                                             \
  bool operator<(const CLASS &other) const noexcept {                           \
    return MEMBER < other.MEMBER;                                               \
  }                                                                             \
  bool operator<=(const CLASS &other) const noexcept {                          \
    return MEMBER <= other.MEMBER;                                              \
  }                                                                             \
  bool operator>(const CLASS &other) const noexcept {                           \
    return MEMBER > other.MEMBER;                                               \
  }                                                                             \
  bool operator>=(const CLASS &other) const noexcept {                          \
    return MEMBER >= other.MEMBER;                                              \
  }                                                                             \
                                                                                \
  bool operator==(std::nullptr_t) const noexcept {                              \
    return MEMBER == TYPE();                                                    \
  }                                                                             \
  bool operator!=(std::nullptr_t) const noexcept {                              \
    return MEMBER != TYPE();                                                    \
  }                                                                             \
  bool operator<(std::nullptr_t) const noexcept {                               \
    return MEMBER < TYPE();                                                     \
  }                                                                             \
  bool operator<=(std::nullptr_t) const noexcept {                              \
    return MEMBER <= TYPE();                                                    \
  }                                                                             \
  bool operator>(std::nullptr_t) const noexcept {                               \
    return MEMBER > TYPE();                                                     \
  }                                                                             \
  bool operator>=(std::nullptr_t) const noexcept {                              \
    return MEMBER >= TYPE();                                                    \
  }

#endif
