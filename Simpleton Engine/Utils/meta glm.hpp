//
//  meta glm.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 19/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_meta_glm_hpp
#define engine_utils_meta_glm_hpp

#include <glm/glm.hpp>

namespace Utils {
  template <typename Any>
  struct VecTraits {
    static constexpr bool isVec = false;
  };
  
  #define VEC_TRAITS(SIZE)                                                      \
    template <typename ValueType, glm::precision PRECISION>                     \
    struct VecTraits<glm::tvec##SIZE<ValueType, PRECISION>> {                   \
      static constexpr bool isVec = true;                                       \
      static constexpr size_t size = SIZE;                                      \
      using valueType = ValueType;                                              \
    }
 
  VEC_TRAITS(1);
  VEC_TRAITS(2);
  VEC_TRAITS(3);
  VEC_TRAITS(4);
  
  #undef VEC_TRAITS
  
  template <typename Vec>
  constexpr bool is_vec = VecTraits<Vec>::isVec;
  
  template <typename Vec>
  constexpr size_t vec_size = VecTraits<Vec>::size;
  
  template <typename Vec>
  using vec_value_type = typename VecTraits<Vec>::valueType;
  
  template <typename Any>
  struct MatTraits {
    static constexpr bool isMat = false;
  };
  
  #define MAT_TRAITS(ROWS, COLS)                                                \
    template <typename ValueType, glm::precision PRECISION>                     \
    struct MatTraits<glm::tmat##ROWS##x##COLS<ValueType, PRECISION>> {          \
      static constexpr bool isMat = true;                                       \
      static constexpr size_t rows = ROWS;                                      \
      static constexpr size_t cols = COLS;                                      \
      using valueType = ValueType;                                              \
    };
  
  MAT_TRAITS(2, 2);
  MAT_TRAITS(2, 3);
  MAT_TRAITS(2, 4);
  MAT_TRAITS(3, 2);
  MAT_TRAITS(3, 3);
  MAT_TRAITS(3, 4);
  MAT_TRAITS(4, 2);
  MAT_TRAITS(4, 3);
  MAT_TRAITS(4, 4);
  
  #undef MAT_TRAITS
  
  template <typename Mat>
  constexpr bool is_mat = MatTraits<Mat>::isMat;
  
  template <typename Mat>
  constexpr size_t mat_rows = MatTraits<Mat>::rows;
  
  template <typename Mat>
  constexpr size_t mat_cols = MatTraits<Mat>::cols;
  
  template <typename Mat>
  constexpr size_t mat_cells = mat_rows<Mat> * mat_cols<Mat>;
  
  template <typename Mat>
  using mat_value_type = typename MatTraits<Mat>::valueType;
}

#endif
