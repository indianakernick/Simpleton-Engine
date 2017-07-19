//
//  dir.hpp
//  Game Engine
//
//  Created by Indi Kernick on 10/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_math_dir_hpp
#define engine_math_dir_hpp

#include <stdexcept>
#include <glm/vec2.hpp>

namespace Math {
  ///The underlying type of Dir and Axis
  using DirType = uint8_t;
  
  using SignedDirType = std::make_signed_t<DirType>;
  
  ///A 2D orthogonal direction
  enum class Dir : DirType {
    //don't reoder this
    
    UP,
    RIGHT,
    DOWN,
    LEFT,
    
    ///Passing NONE to any function other than filterNone or filterNoneCustom
    ///is undefined behaviour
    NONE = 0b11111100
  };
  
  enum class Axis : DirType {
    //don't reoder this
    
    VERT,
    VERTICAL = VERT,
    
    HORI,
    HORIZONTAL = HORI,
  };
  
  ///Ensure that a Dir is not Dir::NONE by returning Dir::UP instead of Dir::NONE
  constexpr Dir filterNone(const Dir dir) {
    return static_cast<Dir>(static_cast<DirType>(dir) & DirType(0b11));
  }
  
  ///Ensure that a Dir is not Dir::NONE by returning a custom Dir instead of
  ///Dir::NONE
  constexpr Dir filterNoneCustom(const Dir dir, const Dir noneDir) {
    return dir == Dir::NONE ? noneDir : dir;
  }
  
  ///Get the opposite of a direction
  constexpr Dir opposite(const Dir dir) {
    //flip the second least significant bit
    return static_cast<Dir>(static_cast<DirType>(dir) ^ DirType(0b10));
  }
  
  ///Get the opposite of an axis
  constexpr Axis opposite(const Axis axis) {
    //flip the least significant bit
    return static_cast<Axis>(static_cast<DirType>(axis) ^ DirType(0b1));
  }
  
  ///Rotate a direction clockwise
  constexpr Dir rotateCW(const Dir dir, const DirType count = 1) {
    return static_cast<Dir>((static_cast<DirType>(dir) + count) & DirType(0b11));
  }
  
  ///Rotate a direction counter-clockwise (anti-clockwise)
  constexpr Dir rotateCCW(const Dir dir, const DirType count = 1) {
    return static_cast<Dir>((static_cast<DirType>(dir) - count) & DirType(0b11));
  }
  
  ///Get the distance between directions
  constexpr DirType dist(const Dir a, const Dir b) {
    const SignedDirType diff = static_cast<SignedDirType>(a) - static_cast<SignedDirType>(b);
    const DirType sign = diff >> (sizeof(DirType) * 8 - 1);
    const DirType dist = (diff ^ sign) + (sign & DirType(1));
    
    //@TODO remove branch
    if (dist == DirType(3)) {
      return DirType(1);
    } else {
      return dist;
    }
  }
  
  ///Get the difference between directions
  constexpr SignedDirType diff(const Dir a, const Dir b) {
    const SignedDirType diff = static_cast<SignedDirType>(b) - static_cast<SignedDirType>(a);
    
    //@TODO remove branch
    if (diff == SignedDirType(3)) {
      return SignedDirType(-1);
    } else if (diff == SignedDirType(-3)) {
      return SignedDirType(1);
    } else {
      return diff;
    }
  }
  
  ///Get the axis that a direction is on
  constexpr Axis getAxis(const Dir dir) {
    //get the least significant bit
    return static_cast<Axis>(static_cast<DirType>(dir) & DirType(0b01));
  }
  
  /*
  This is how convenient these convenience functions are
  
  Math::isVert(dir)
  Math::getAxis(dir) == Math::Axis::VERT
  
  Math::isHori(dir)
  Math::getAxis(dir) == Math::Axis::HORI
  
  Math::sameAxis(dir0, dir1)
  Math::getAxis(dir0) == Math::getAxis(dir1)
  */
  
  ///Determine whether a direction is on the vertical axis
  constexpr bool isVert(const Dir dir) {
    return getAxis(dir) == Axis::VERT;
  }
  
  ///Determine whether a direction is on the horizontal axis
  constexpr bool isHori(const Dir dir) {
    return getAxis(dir) == Axis::HORI;
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
          throw std::invalid_argument("Invalid direction");
      }
    }
  };
  
  ///Configuration template for converting a 2D unit vector to a direction
  template <typename Number, Dir PLUS_X, Dir PLUS_Y>
  struct FromVec {
    static_assert(!sameAxis(PLUS_X, PLUS_Y), "PLUS_X and PLUS_Y must be on different axes");
    static_assert(
      std::is_floating_point<Number>::value || std::is_signed<Number>::value,
      "Number must be a float or a signed int"
    );
  
  private:
    static constexpr Number ONE = Number(1);
    static constexpr Number ZERO = Number(0);
  
  public:
    ///Convert a 2D unit vector to a direction
    static Dir conv(const glm::tvec2<Number> vec, const Number dist = ONE) {
      using Dir = glm::tvec2<Number>;
      if (vec == Dir(dist, ZERO)) {
        return PLUS_X;
      } else if (vec == Dir(-dist, ZERO)) {
        return opposite(PLUS_X);
      } else if (vec == Dir(ZERO, dist)) {
        return PLUS_Y;
      } else if (vec == Dir(ZERO, -dist)) {
        return opposite(PLUS_Y);
      } else {
        throw std::invalid_argument("Vector cannot be converted to direction");
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
  
  ///Configuration template for converting a number to a direction
  template <typename Number>
  struct FromNum {
    static_assert(std::is_arithmetic<Number>::value, "Number must be an arithmetic type");
    
    ///Convert a number to a direction
    static Dir conv(const Number num, const Number stride, const Number offset) {
      return static_cast<Dir>((num - offset) / stride);
    }
    
    ///Convert a number to a direction
    static Dir conv(const Number num, const Number stride) {
      return static_cast<Dir>(num / stride);
    }
    
    ///Convert a number to a direction
    static Dir conv(const Number num) {
      return static_cast<Dir>(num);
    }
  };
}

#endif
