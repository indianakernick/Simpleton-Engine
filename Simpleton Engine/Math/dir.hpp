//
//  dir.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 10/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_math_dir_hpp
#define engine_math_dir_hpp

#include <cctype>
#include <stdexcept>
#include <string_view>
#include <glm/vec2.hpp>

namespace Math {
  ///The underlying type of Dir and Axis
  using DirType = uint8_t;
  
  using SignedDirType = std::make_signed_t<DirType>;
  
  static_assert(std::is_unsigned_v<DirType>);
  static_assert(std::is_signed_v<SignedDirType>);
  
  ///A 2D orthogonal direction
  enum class Dir : DirType {
    //don't reoder this
    
    UP,
    RIGHT,
    DOWN,
    LEFT,
    COUNT,
    
    NORTH = UP,
    EAST = RIGHT,
    SOUTH = DOWN,
    WEST = LEFT,
    
    TOP = UP,
    BOTTOM = DOWN,
    
    FORWARD = UP,
    BACKWARD = DOWN,
    
    MIN = UP,
    MAX = LEFT,
    
    BEGIN = MIN,
    END = COUNT,
    
    R_BEGIN = MAX,
    R_END = std::numeric_limits<DirType>::max(),
    
    ///Passing NONE to any function other than filterNone or filterNoneCustom
    ///is undefined behaviour
    NONE = std::numeric_limits<DirType>::max() - (COUNT - 1)
  };
  
  struct DirRange {
    static constexpr Dir begin() {
      return Dir::BEGIN;
    }
    static constexpr Dir end() {
      return Dir::END;
    }
  };
  
  constexpr DirRange DIR_RANGE = {};
  
  enum class Axis : DirType {
    //don't reoder this
    
    VERT,
    HORI,
    COUNT,
    
    VERTICAL = VERT,
    HORIZONTAL = HORI,
    
    UP_DOWN = VERT,
    LEFT_RIGHT = HORI,
    
    ///Passing NONE to any function other than filterNone or filterNoneCustom
    ///is undefined behaviour
    NONE = std::numeric_limits<DirType>::max() - (COUNT - 1)
  };
  
  ///Ensure that a Dir is not Dir::NONE by returning Dir::UP instead of
  ///Dir::NONE
  constexpr Dir filterNone(const Dir dir) {
    return static_cast<Dir>(static_cast<DirType>(dir) & DirType(0b11));
  }
  
  ///Ensure that a Dir is not Dir::NONE by returning a custom Dir instead of
  ///Dir::NONE
  constexpr Dir filterNoneCustom(const Dir dir, const Dir noneDir) {
    return dir == Dir::NONE ? noneDir : dir;
  }
  
  ///Ensure that an Axis is not Axis::NONE by returning Axis::UP instead of
  ///Axis::NONE
  constexpr Axis filterNone(const Axis axis) {
    return static_cast<Axis>(static_cast<DirType>(axis) & DirType(0b1));
  }
  
  ///Ensure that n Axis is not Axis::NONE by returning a custom Axis instead of
  ///Axis::NONE
  constexpr Axis filterNoneCustom(const Axis axis, const Axis noneAxis) {
    return axis == Axis::NONE ? noneAxis : axis;
  }
  
  ///Check if a direction is valid
  constexpr bool valid(const Dir dir) {
    return dir == Dir::UP ||
           dir == Dir::RIGHT ||
           dir == Dir::DOWN ||
           dir == Dir::LEFT;
  }
  
  ///Check if a direction is either valid or equal to Dir::NONE
  constexpr bool validOrNone(const Dir dir) {
    return valid(dir) || dir == Dir::NONE;
  }
  
  ///Check if an axis is valid
  constexpr bool valid(const Axis axis) {
    return axis == Axis::VERT ||
           axis == Axis::HORI;
  }
  
  ///Check if an axis is either valid or equal to Axis::NONE
  constexpr bool validOrNone(const Axis axis) {
    return valid(axis) || axis == Axis::NONE;
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
  
  ///Rotate a direction. Clockwise if count is positive and counter-clockwise
  ///(anti-clockwise) if count is negative
  constexpr Dir rotate(const Dir dir, const SignedDirType count) {
    return rotateCW(dir, static_cast<DirType>(count));
  }
  
  ///Get the distance between directions
  constexpr DirType dist(const Dir a, const Dir b) {
    constexpr DirType distances[16] = {
      0, 1, 2, 1, 1, 0, 1, 2, 2, 1, 0, 1, 1, 2, 1, 0
    };
    return distances[
      (static_cast<DirType>(a) << 2) |
      static_cast<DirType>(b)
    ];
  }
  
  ///Get the difference between directions
  constexpr SignedDirType diff(const Dir a, const Dir b) {
    constexpr SignedDirType differences[16] = {
      0, 1, 2, -1, -1, 0, 1, 2, -2, -1, 0, 1, 1, -2, -1, 0
    };
    return differences[
      (static_cast<DirType>(a) << 2) |
      static_cast<DirType>(b)
    ];
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
  template <typename Number_, Dir PLUS_X_ = Dir::RIGHT, Dir PLUS_Y_ = Dir::UP>
  struct ToVec {
    using Number = Number_;
    static constexpr Dir PLUS_X = PLUS_X_;
    static constexpr Dir PLUS_Y = PLUS_Y_;
  
    static_assert(!sameAxis(PLUS_X, PLUS_Y), "PLUS_X and PLUS_Y must be on different axes");
  
    ///Convert a direction to a 2D unit vector
    static glm::tvec2<Number> conv(const Dir dir, const Number dist = Number(1)) {
      constexpr Number ZERO(0);
      
      switch (dir) {
        case PLUS_X:
          return {dist, ZERO};
        case opposite(PLUS_X):
          return {-dist, ZERO};
        case PLUS_Y:
          return {ZERO, dist};
        case opposite(PLUS_Y):
          return {ZERO, -dist};
        case Dir::NONE:
          return {ZERO, ZERO};
        
        default:
          throw std::invalid_argument("Invalid direction");
      }
    }
  };
  
  ///Configuration template for converting a 2D unit vector to a direction
  template <typename Number_, Dir PLUS_X_ = Dir::RIGHT, Dir PLUS_Y_ = Dir::UP, bool EXACT_ = true>
  struct FromVec {
    using Number = Number_;
    static constexpr Dir PLUS_X = PLUS_X_;
    static constexpr Dir PLUS_Y = PLUS_Y_;
    static constexpr bool EXACT = EXACT_;
  
    static_assert(!sameAxis(PLUS_X, PLUS_Y), "PLUS_X and PLUS_Y must be on different axes");
    
    ///Convert a 2D unit vector to a direction
    static Dir conv(const glm::tvec2<Number> vec, const Number dist = Number(1)) {
      using Vec = glm::tvec2<Number>;
      constexpr Number ZERO(0);
      
      if constexpr (EXACT) {
      
               if (vec == Vec(dist, ZERO)) {
          return PLUS_X;
        } else if (vec == Vec(-dist, ZERO)) {
          return opposite(PLUS_X);
        } else if (vec == Vec(ZERO, dist)) {
          return PLUS_Y;
        } else if (vec == Vec(ZERO, -dist)) {
          return opposite(PLUS_Y);
        } else {
          throw std::invalid_argument("Vector cannot be converted to direction");
        }
        
      } else {
        
        if (vec.x == vec.y) {
          throw std::invalid_argument("Vector cannot be converted to direction");
        }
        if (std::abs(vec.x) > std::abs(vec.y)) {
          if (vec.x > ZERO) {
            return PLUS_X;
          } else {
            return opposite(PLUS_X);
          }
        } else {
          if (vec.y > ZERO) {
            return PLUS_Y;
          } else {
            return opposite(PLUS_Y);
          }
        }
        
      }
    }
  };
  
  ///Convert a direction to a number
  template <typename Number>
  Number toNum(const Dir dir) {
    assert(valid(dir));
    return static_cast<Number>(dir);
  }
  
  ///Convert a direction to a number and multiply
  template <typename Number>
  Number toNum(const Dir dir, const Number factor) {
    assert(valid(dir));
    return static_cast<Number>(dir) * factor;
  }
  
  ///Convert a number to a direction
  template <typename Number>
  Dir toDir(const Number number) {
    const Dir dir = static_cast<Dir>(number);
    assert(valid(dir));
    return dir;
  }
  
  constexpr std::string_view toUpperCaseString(const Dir dir) {
    switch (dir) {
      case Dir::UP:
        return "UP";
      case Dir::RIGHT:
        return "RIGHT";
      case Dir::DOWN:
        return "DOWN";
      case Dir::LEFT:
        return "LEFT";
      case Dir::NONE:
        return "NONE";
      default:
        return "";
    }
  }
  
  constexpr std::string_view toLowerCaseString(const Dir dir) {
    switch (dir) {
      case Dir::UP:
        return "up";
      case Dir::RIGHT:
        return "right";
      case Dir::DOWN:
        return "down";
      case Dir::LEFT:
        return "left";
      case Dir::NONE:
        return "none";
      default:
        return "";
    }
  }
}

#include <cmath>

namespace Math {
  // might potentially be useful
  inline glm::vec2 roundInDir(const glm::vec2 vec, const Math::Dir dir) {
    constexpr Math::Dir PLUS_X = Math::Dir::RIGHT;
    constexpr Math::Dir PLUS_Y = Math::Dir::UP;
    
    switch (dir) {
      case PLUS_X:
        return {std::ceil(vec.x), vec.y};
      case PLUS_Y:
        return {vec.x, std::ceil(vec.y)};
      case Math::opposite(PLUS_X):
        return {std::floor(vec.x), vec.y};
      case Math::opposite(PLUS_Y):
        return {vec.x, std::floor(vec.y)};
      case Math::Dir::NONE:
        return vec;
      
      default:
        assert(false);
    }
  }
  
  constexpr Dir toDir(const std::string_view str) {
    //Case insensitive comparison. Assumes other is lower case
    const auto equalTo = [str] (const std::string_view other) {
      if (str.size() != other.size()) {
        return false;
      }
      for (size_t i = 0; i != str.size(); ++i) {
        if (std::tolower(str[i]) != other[i]) {
          return false;
        }
      }
      return true;
    };
    
    if (str.size() == 0) {
      return Dir::NONE;
    } else if (equalTo("up")) {
      return Dir::UP;
    } else if (equalTo("right")) {
      return Dir::RIGHT;
    } else if (equalTo("down")) {
      return Dir::DOWN;
    } else if (equalTo("left")) {
      return Dir::LEFT;
    } else {
      return Dir::NONE;
    }
  }
  
  ///Get the next direction. Used for iterating directions
  constexpr Dir next(const Dir dir) {
    return static_cast<Dir>(static_cast<DirType>(dir) + DirType(1));
  }
  
  ///Get the previous direction. Used for iterating directions in reverse
  constexpr Dir prev(const Dir dir) {
    return static_cast<Dir>(static_cast<DirType>(dir) - DirType(1));
  }
  
  ///Get the next direction. Used for iterating directions
  constexpr Math::Dir &operator++(Math::Dir &dir) {
    return dir = Math::next(dir);
  }

  ///Get the previous direction. Used for iterating directions in reverse
  constexpr Math::Dir &operator--(Math::Dir &dir) {
    return dir = Math::prev(dir);
  }
  
  ///Dereference a direction. Used for iterating directions
  constexpr Math::Dir operator*(const Math::Dir dir) {
    return dir;
  }
}

#endif
