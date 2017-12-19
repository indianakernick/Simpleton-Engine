//
//  generic raii.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_generic_raii_hpp
#define engine_opengl_generic_raii_hpp

#include <utility>

// Same interface as std::unique_ptr (the best class in the entire STL)

#define RAII_CLASS_MEMBERS(CLASS, TYPE, MEMBER, DELETE)                         \
  CLASS()                                                                       \
    : MEMBER() {}                                                               \
                                                                                \
  CLASS(std::nullptr_t)                                                         \
    : MEMBER() {}                                                               \
                                                                                \
  explicit CLASS(const TYPE MEMBER)                                             \
    : MEMBER(MEMBER) {}                                                         \
                                                                                \
  CLASS(CLASS &&other)                                                          \
    : MEMBER(other.release()) {}                                                \
                                                                                \
  CLASS &operator=(CLASS &&other) {                                             \
    MEMBER = other.release();                                                   \
    return *this;                                                               \
  }                                                                             \
                                                                                \
  CLASS &operator=(std::nullptr_t) {                                            \
    DELETE(MEMBER);                                                             \
    MEMBER = TYPE();                                                            \
    return *this;                                                               \
  }                                                                             \
                                                                                \
  ~CLASS() {                                                                    \
    DELETE(MEMBER);                                                             \
  }                                                                             \
                                                                                \
  TYPE release() {                                                              \
    return std::exchange(MEMBER, TYPE());                                       \
  }                                                                             \
                                                                                \
  void reset(const TYPE new##MEMBER = TYPE()) {                                 \
    DELETE(MEMBER);                                                             \
    MEMBER = new##MEMBER;                                                       \
  }                                                                             \
                                                                                \
  void swap(CLASS &other) {                                                     \
    std::swap(MEMBER, other.MEMBER);                                            \
  }                                                                             \
                                                                                \
  TYPE get() const {                                                            \
    return MEMBER;                                                              \
  }                                                                             \
                                                                                \
  explicit operator bool() const {                                              \
    return MEMBER != TYPE();                                                    \
  }

#endif
