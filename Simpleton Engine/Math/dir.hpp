//
//  dir.hpp
//  Game Engine
//
//  Created by Indi Kernick on 10/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_math_dir_hpp
#define engine_math_dir_hpp

#include <cassert>
#include <glm/vec2.hpp>

namespace Math {
  ///The underlying type of Dir and Axis
  using DirType = uint8_t;
  
  ///A 2D orthogonal direction
  enum class Dir : DirType {
    //don't reoder this
    
    UP,
    RIGHT,
    DOWN,
    LEFT
  };
  
  enum class Axis : DirType {
    //don't reoder this
    
    VERT,
    VERTICAL = VERT,
    
    HORI,
    HORIZONTAL = HORI,
  };
  
  ///Get the opposite of a direction
  constexpr Dir opposite(const Dir dir) {
    //flip the second least significant bit
    return static_cast<Dir>(static_cast<DirType>(dir) ^ DirType(0b10));
  }
  
  ///Get the opposite of an axis
  constexpr Axis opposite(const Axis axis) {
    //flip the least significant bit
    return static_cast<Axis>(static_cast<DirType>(axis) ^ DirType(0b01));
  }
  
  ///Rotate a direction clockwise
  constexpr Dir rotateCW(const Dir dir, const DirType count = 1) {
    return static_cast<Dir>((static_cast<DirType>(dir) + count) & DirType(0b11));
  }
  
  ///Rotate a direction counter-clockwise (anti-clockwise)
  constexpr Dir rotateCCW(const Dir dir, const DirType count = 1) {
    return static_cast<Dir>((static_cast<DirType>(dir) - count) & DirType(0b11));
  }
  
  ///Get the axis that a direction is on
  constexpr Axis getAxis(const Dir dir) {
    //get the least significant bit
    return static_cast<Axis>(static_cast<DirType>(dir) & DirType(0b01));
  }
  
  ///Are these directions on the same axis?
  constexpr bool sameAxis(const Dir a, const Dir b) {
    return getAxis(a) == getAxis(b);
  }
  
  ///Configuration template for converting a direction to a 2D unit vector
  template <typename Number, Dir PLUS_X, Dir PLUS_Y>
  struct ToVec {
    static_assert(!sameAxis(PLUS_X, PLUS_Y), "PLUS_X and PLUS_Y must be on different axes");
    static_assert(
      std::is_floating_point<Number>::value || std::is_signed<Number>::value,
      "Number must be a float or a signed int"
    );
  
  private:
    static constexpr Number ONE = Number(1);
    static constexpr Number ZERO = Number(0);
  
  public:
    ///Convert a direction to a 2D unit vector
    static glm::tvec2<Number> conv(const Dir dir, const Number dist = ONE) {
      switch (dir) {
        case PLUS_X:
          return {dist, ZERO};
        case opposite(PLUS_X):
          return {-dist, ZERO};
        case PLUS_Y:
          return {ZERO, dist};
        case opposite(PLUS_Y):
          return {ZERO, -dist};
        
        default:
          assert(false);
      }
    }
  };
  
  ///Configuration template for converting a direction to a number
  template <typename Number>
  struct ToNum {
    static_assert(std::is_arithmetic<Number>::value, "Number must be an arithmetic type");
    
    ///Convert a direction to a number
    static Number conv(const Dir dir, const Number stride, const Number offset) {
      return static_cast<Number>(dir) * stride + offset;
    }
    
    ///Convert a direction to a number
    static Number conv(const Dir dir, const Number stride) {
      return static_cast<Number>(dir) * stride;
    }
    
    ///Convert a direction to a number
    static Number conv(const Dir dir) {
      return static_cast<Number>(dir);
    }
  };
}

#endif
