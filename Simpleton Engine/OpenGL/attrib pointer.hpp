//
//  attrib pointer.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_opengl_attrib_pointer_hpp
#define engine_opengl_attrib_pointer_hpp

#include "type enum.hpp"
#include "../Utils/meta glm.hpp"

namespace GL {
  namespace detail {
    template <typename T, bool INT, bool NORM>
    void attribPointerVecImpl(const GLint attr, const size_t stride, const size_t offset) {
      if constexpr (INT) {
        glVertexAttribIPointer(
          attr,
          TypeEnum<T>::size,
          TypeEnum<T>::type,
          static_cast<GLsizei>(stride),
          reinterpret_cast<const GLvoid *>(offset)
        );
        CHECK_OPENGL_ERROR();
      } else {
        glVertexAttribPointer(
          attr,
          TypeEnum<T>::size,
          TypeEnum<T>::type,
          NORM ? GL_TRUE : GL_FALSE,
          static_cast<GLsizei>(stride),
          reinterpret_cast<const GLvoid *>(offset)
        );
        CHECK_OPENGL_ERROR();
      }
    }
    
    enum class AttribMode {
      // glVertexAttribPointer for floats and glVertexAttribIPointer for ints
      NO_CHANGE,
      // glVertexAttribPointer with no normalization
      TO_FLOAT,
      // glVertexAttribPointer with normalization
      FIXED_POINT
    };
    
    template <typename T, AttribMode MODE>
    void attribPointerVec(const GLint attr, const size_t stride, const size_t offset) {
      if constexpr (MODE == AttribMode::NO_CHANGE) {
        if constexpr (std::is_floating_point_v<T>) {
          attribPointerVecImpl<T, false, false>(attr, stride, offset);
        } else if constexpr (std::is_integral_v<T>) {
          attribPointerVecImpl<T, true, false>(attr, stride, offset);
        }
      } else if constexpr (MODE == AttribMode::TO_FLOAT) {
        attribPointerVecImpl<T, false, false>(attr, stride, offset);
      } else if constexpr (MODE == AttribMode::FIXED_POINT) {
        attribPointerVecImpl<T, false, true>(attr, stride, offset);
      }
    }
    
    template <typename T>
    constexpr GLint numAttrLocations() {
      if constexpr (std::is_arithmetic_v<T> || Utils::is_vec<T>) {
        return 1;
      } else if constexpr (Utils::is_mat<T>) {
        return Utils::mat_cols<T>;
      } else if constexpr (std::is_array_v<T>) {
        return std::extent_v<T, 0> * numAttrLocations<std::remove_extent_t<T>>();
      }
    }
    
    template <typename, AttribMode>
    void attribPointer(GLint, size_t, size_t);
    
    template <typename Type, typename ValueType, AttribMode MODE>
    void attribPointerArray(const GLint attr, size_t stride, size_t offset) {
      constexpr GLint SIZE = numAttrLocations<Type>();
      constexpr GLint VALUE_SIZE = numAttrLocations<ValueType>();
    
      stride = (stride == 0 ? sizeof(Type) : stride);
      for (GLint a = attr; a != attr + SIZE; a += VALUE_SIZE) {
        attribPointer<ValueType, MODE>(a, stride, offset);
        offset += sizeof(ValueType);
      }
    }
    
    template <typename T, AttribMode MODE>
    void attribPointer(const GLint attr, const size_t stride, const size_t offset) {
      if constexpr (Utils::is_vec<T> || std::is_arithmetic_v<T>) {
        attribPointerVec<T, MODE>(attr, stride, offset);
      } else if constexpr (Utils::is_mat<T>) {
        attribPointerArray<T, T::col_type, MODE>(attr, stride, offset);
      } else if constexpr (std::is_array_v<T>) {
        attribPointerArray<T, std::remove_extent<T>, MODE>(attr, stride, offset);
      }
    }
  }
  
  template <GLint ATTR, typename T>
  void enable() {
    constexpr GLint SIZE = detail::numAttrLocations<T>();
    for (GLint a = ATTR; a != ATTR + SIZE; ++a) {
      glEnableVertexAttribArray(a);
      CHECK_OPENGL_ERROR();
    }
  }
  
  template <GLint ATTR, typename T>
  void disable() {
    constexpr GLint SIZE = detail::numAttrLocations<T>();
    for (GLint a = ATTR; a != ATTR + SIZE; ++a) {
      glDisableVertexAttribArray(a);
      CHECK_OPENGL_ERROR();
    }
  }
  
  /// Passes ints as ints and floats as floats
  template <GLint ATTR, typename T>
  void attribPointer(const size_t stride, const size_t offset) {
    detail::attribPointer<T, detail::AttribMode::NO_CHANGE>(ATTR, stride, offset);
  }
  
  /// Passes ints as floats and floats as floats
  template <GLint ATTR, typename T>
  void attribPointerFloat(const size_t stride, const size_t offset) {
    detail::attribPointer<T, detail::AttribMode::TO_FLOAT>(ATTR, stride, offset);
  }
  
  /// Passes ints as normalized floats and floats as floats
  template <GLint ATTR, typename T>
  void attribPointerFixedPoint(const size_t stride, const size_t offset) {
    detail::attribPointer<T, detail::AttribMode::FIXED_POINT>(ATTR, stride, offset);
  }
}

#endif
