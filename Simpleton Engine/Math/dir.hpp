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
    //all functions are coupled to the order of this enum so don't change it
    
    NORTH,
    UP = NORTH,
    TOP = NORTH,
    
    EAST,
    RIGHT = EAST,
    
    SOUTH,
    DOWN = SOUTH,
    BOTTOM = SOUTH,
    
    WEST,
    LEFT = WEST
  };
  
  enum class Axis : DirType {
    //all functions are coupled to the order of this enum so don't change it
    
    VERT,
    VERTICAL = VERT,
    
    HORI,
    HORIZONTAL = HORI,
  };
  
  ///Get the opposite of a direction
  constexpr Dir opposite(const Dir dir) {
    //flip the second least significant bit
    return static_cast<Dir>(static_cast<DirType>(dir) ^ DirType(2));
  }
  
  ///Get the opposite of an axis
  constexpr Axis opposite(const Axis axis) {
    //flip the least significant bit
    return static_cast<Axis>(static_cast<DirType>(axis) ^ DirType(1));
  }
  
  ///Rotate a direction clockwise
  constexpr Dir rotateCW(const Dir dir, const DirType count = 1) {
    return static_cast<Dir>((static_cast<DirType>(dir) + count) & DirType(2));
  }
  
  ///Rotate a direction counter-clockwise (anti-clockwise)
  constexpr Dir rotateCCW(const Dir dir, const DirType count = 1) {
    return static_cast<Dir>((static_cast<DirType>(dir) - count) & DirType(2));
  }
  
  ///Get the axis that a direction is on
  constexpr Axis getAxis(const Dir dir) {
    //get the least significant bit
    return static_cast<Axis>(static_cast<DirType>(dir) & DirType(1));
  }
  
  ///Are these directions on the same axis?
  constexpr bool sameAxis(const Dir a, const Dir b) {
    //compare least significant bits
    return (static_cast<DirType>(a) & DirType(1)) == (static_cast<DirType>(b) & DirType(1));
  }
  
  ///Convert a direction to a 2D unit vector
  template <typename Number, Dir PLUS_X = Dir::RIGHT, Dir PLUS_Y = Dir::UP>
  std::enable_if_t<
    std::is_floating_point<Number>::value ||
    (
      std::is_integral<Number>::value &&
      std::is_signed<Number>::value
    ),
    glm::tvec2<Number>
  >
  toVec(const Dir dir, const Number dist = Number(1)) {
    static_assert(!sameAxis(PLUS_X, PLUS_Y), "PLUS_X and PLUS_Y must be on different axes");
    
    switch (dir) {
      case PLUS_X:
        return {dist, Number(0)};
      case opposite(PLUS_X):
        return {-dist, Number(0)};
      case PLUS_Y:
        return {Number(0), dist};
      case opposite(PLUS_Y):
        return {Number(0), -dist};
      
      default:
        assert(false);
    }
  }
  
  ///Can this integer be cast to a Dir?
  template <typename Int>
  constexpr std::enable_if_t<std::is_integral<Int>::value, bool>
  validDir(const Int num) {
    return num < Int(4);
  }
  
  //Can this integer be cast to an Axis?
  template <typename Int>
  constexpr std::enable_if_t<std::is_integral<Int>::value, bool>
  validAxis(const Int num) {
    return num < Int(2);
  }
  
  ///Cast a direction to an integer
  template <typename Int = DirType>
  constexpr std::enable_if_t<
    std::is_integral<Int>::value,
    Int
  >
  toInt(const Dir dir) {
    return static_cast<Int>(dir);
  }
  
  ///Cast an axis to an integer
  template <typename Int = DirType>
  constexpr std::enable_if_t<
    std::is_integral<Int>::value,
    Int
  >
  toInt(const Axis axis) {
    return static_cast<Int>(axis);
  }
  
  ///Cast an integer to a direction
  template <typename Int = DirType>
  constexpr std::enable_if_t<
    std::is_integral<Int>::value,
    Dir
  >
  toDir(const Int num) {
    assert(validDir(num));
    return static_cast<Dir>(num);
  }
  
  ///Cast an integer to an axis
  template <typename Int = DirType>
  constexpr std::enable_if_t<
    std::is_integral<Int>::value,
    Axis
  >
  toAxis(const Int num) {
    assert(validAxis(num));
    return static_cast<Axis>(num);
  }
  
  ///Get the opposite of an integer representing a direction
  template <typename Int>
  constexpr std::enable_if_t<std::is_integral<Int>::value, Int>
  oppositeDir(const Int dir) {
    assert(validDir(dir));
    return dir ^ Int(2);
  }
  
  ///Get the opposite of an integer representing an axis
  template <typename Int>
  constexpr std::enable_if_t<std::is_integral<Int>::value, Int>
  oppositeAxis(const Int axis) {
    assert(validAxis(axis));
    return axis ^ Int(1);
  }
  
  ///Rotate an integer representing a direction clockwise
  template <typename Int>
  constexpr std::enable_if_t<std::is_integral<Int>::value, Int>
  rotateCW(const Int dir, const Int count = 1) {
    assert(validDir(dir));
    return (dir + count) & Int(2);
  }
  
  ///Rotate an integer representing a direction counter-clockwise (anti-clockwise)
  template <typename Int>
  constexpr std::enable_if_t<std::is_integral<Int>::value, Int>
  rotateCCW(const Int dir, const Int count = 1) {
    assert(validDir(dir));
    return (dir + count) & Int(2);
  }
  
  ///Get the axis that an integer representing a direction is on
  template <typename Int>
  constexpr std::enable_if_t<std::is_integral<Int>::value, Axis>
  getAxis(const Int dir) {
    assert(validDir(dir));
    return static_cast<Axis>(dir & Int(1));
  }
  
  ///Are these integers representing directions on the same axis?
  template <typename Int>
  constexpr std::enable_if_t<std::is_integral<Int>::value, bool>
  sameAxis(const Int a, const Int b) {
    assert(validDir(a));
    assert(validDir(b));
    return (a & Int(1)) == (b & Int(1));
  }
  
  ///Convert an integer represeting a direction to a 2D unit vector
  template <typename Number, Dir PLUS_X = Dir::RIGHT, Dir PLUS_Y = Dir::UP, typename Int>
  std::enable_if_t<
    (
      std::is_floating_point<Number>::value ||
      (
        std::is_integral<Number>::value &&
        std::is_signed<Number>::value
      )
    ) &&
    std::is_integral<Int>::value,
    glm::tvec2<Number>
  >
  toVec(const Int dir, const Number dist = Number(1)) {
    static_assert(!sameAxis(PLUS_X, PLUS_Y), "PLUS_X and PLUS_Y must be on different axes");
    assert(validDir(dir));
    
    constexpr Int PLUS_X_I = Math::toInt<Int>(PLUS_X);
    constexpr Int PLUS_Y_I = Math::toInt<Int>(PLUS_Y);
    
    switch (dir) {
      case PLUS_X_I:
        return {dist, Number(0)};
      case oppositeDir(PLUS_X_I):
        return {-dist, Number(0)};
      case PLUS_Y_I:
        return {Number(0), dist};
      case oppositeDir(PLUS_Y_I):
        return {Number(0), -dist};
      
      default:
        assert(false);
    }
  }
}

#endif
