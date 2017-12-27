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
  using value_type = TYPE;                                                      \
                                                                                \
  CLASS() noexcept                                                              \
    : MEMBER() {}                                                               \
  CLASS(std::nullptr_t) noexcept                                                \
    : MEMBER() {}                                                               \
  explicit CLASS(const value_type MEMBER) noexcept                              \
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
  value_type release() noexcept {                                               \
    return std::exchange(MEMBER, value_type());                                 \
  }                                                                             \
  void reset(const value_type new##MEMBER = {}) noexcept {                      \
    DELETE(MEMBER);                                                             \
    MEMBER = new##MEMBER;                                                       \
  }                                                                             \
  void swap(CLASS &other) noexcept {                                            \
    std::swap(MEMBER, other.MEMBER);                                            \
  }                                                                             \
                                                                                \
  value_type get() const noexcept {                                             \
    return MEMBER;                                                              \
  }                                                                             \
  explicit operator bool() const noexcept {                                     \
    return MEMBER != value_type();                                              \
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
    return MEMBER == value_type();                                              \
  }                                                                             \
  bool operator!=(std::nullptr_t) const noexcept {                              \
    return MEMBER != value_type();                                              \
  }                                                                             \
  bool operator<(std::nullptr_t) const noexcept {                               \
    return MEMBER < value_type();                                               \
  }                                                                             \
  bool operator<=(std::nullptr_t) const noexcept {                              \
    return MEMBER <= value_type();                                              \
  }                                                                             \
  bool operator>(std::nullptr_t) const noexcept {                               \
    return MEMBER > value_type();                                               \
  }                                                                             \
  bool operator>=(std::nullptr_t) const noexcept {                              \
    return MEMBER >= value_type();                                              \
  }

#endif
