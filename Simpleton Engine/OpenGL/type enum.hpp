//
//  type enum.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_type_enum_hpp
#define engine_opengl_type_enum_hpp

#include "opengl.hpp"
#include <type_traits>

namespace GL {
  template <typename T>
  struct TypeEnum {};
  
  #define TYPE_ENUM(TYPE, ENUM)                                                 \
    template <>                                                                 \
    struct TypeEnum<TYPE> {                                                     \
      static constexpr GLenum type = ENUM;                                      \
      static constexpr GLsizei size = 1;                                        \
    };
  
  #define VECTOR_TYPE_ENUM(TYPE, SIZE)                                          \
    template <>                                                                 \
    struct TypeEnum<TYPE> {                                                     \
      static constexpr GLenum type = TypeEnum<typename TYPE::value_type>::type; \
      static constexpr GLsizei size = SIZE;                                     \
    };
  
  #define MATRIX_TYPE_ENUM(TYPE, ROWS, COLS)                                    \
    template <>                                                                 \
    struct TypeEnum<TYPE> {                                                     \
      static constexpr GLenum type = TypeEnum<typename TYPE::value_type>::type; \
      static constexpr GLsizei rows = ROWS;                                     \
      static constexpr GLsizei cols = COLS;                                     \
    };
  
  #define MULTI_VECTOR_TYPE_ENUM(SIZE)                                          \
    VECTOR_TYPE_ENUM(glm::tvec##SIZE<bool>, SIZE)                               \
    VECTOR_TYPE_ENUM(glm::tvec##SIZE<GLbyte>, SIZE)                             \
    VECTOR_TYPE_ENUM(glm::tvec##SIZE<GLubyte>, SIZE)                            \
    VECTOR_TYPE_ENUM(glm::tvec##SIZE<GLshort>, SIZE)                            \
    VECTOR_TYPE_ENUM(glm::tvec##SIZE<GLushort>, SIZE)                           \
    VECTOR_TYPE_ENUM(glm::tvec##SIZE<GLint>, SIZE)                              \
    VECTOR_TYPE_ENUM(glm::tvec##SIZE<GLuint>, SIZE)                             \
    VECTOR_TYPE_ENUM(glm::tvec##SIZE<GLfloat>, SIZE)                            \
    VECTOR_TYPE_ENUM(glm::tvec##SIZE<GLdouble>, SIZE)                           \

  #define MULTI_MATRIX_TYPE_ENUM(ROWS, COLS)                                    \
    MATRIX_TYPE_ENUM(glm::tmat##ROWS##x##COLS<GLfloat>, ROWS, COLS)             \
    MATRIX_TYPE_ENUM(glm::tmat##ROWS##x##COLS<GLdouble>, ROWS, COLS)
    
  //GLboolean is defined as a uint8_t so the types conflict
  
  TYPE_ENUM(bool, GL_BOOL)
  TYPE_ENUM(GLbyte, GL_BYTE)
  TYPE_ENUM(GLubyte, GL_UNSIGNED_BYTE)
  TYPE_ENUM(GLshort, GL_SHORT)
  TYPE_ENUM(GLushort, GL_UNSIGNED_SHORT)
  TYPE_ENUM(GLint, GL_INT)
  TYPE_ENUM(GLuint, GL_UNSIGNED_INT)
  TYPE_ENUM(GLfloat, GL_FLOAT)
  TYPE_ENUM(GLdouble, GL_DOUBLE)
  
  MULTI_VECTOR_TYPE_ENUM(2)
  MULTI_VECTOR_TYPE_ENUM(3)
  MULTI_VECTOR_TYPE_ENUM(4)
  
  MULTI_MATRIX_TYPE_ENUM(2, 2)
  MULTI_MATRIX_TYPE_ENUM(2, 3)
  MULTI_MATRIX_TYPE_ENUM(2, 4)
  MULTI_MATRIX_TYPE_ENUM(3, 2)
  MULTI_MATRIX_TYPE_ENUM(3, 3)
  MULTI_MATRIX_TYPE_ENUM(3, 4)
  MULTI_MATRIX_TYPE_ENUM(4, 2)
  MULTI_MATRIX_TYPE_ENUM(4, 3)
  MULTI_MATRIX_TYPE_ENUM(4, 4)
  
  #undef TYPE_ENUM
  #undef VECTOR_TYPE_ENUM
  #undef MATRIX_TYPE_ENUM
  #undef MULTI_VECTOR_TYPE_ENUM
  #undef MULTI_MATRIX_TYPE_ENUM
}

#endif
