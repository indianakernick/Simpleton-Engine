//
//  dir bits.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 1/1/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_math_dir_bits_hpp
#define engine_math_dir_bits_hpp

#include "dir.hpp"
#include "bits.hpp"

namespace Math {
  ///A bitset where each bit corresponds to a 2D orthogonal direction
  enum class DirBits : DirType {
    //don't reoder this
  
    NONE,
    UP,
    RIGHT,
    UP_RIGHT,
    DOWN,
    UP_DOWN,
    RIGHT_DOWN,
    UP_RIGHT_DOWN,
    LEFT,
    UP_LEFT,
    RIGHT_LEFT,
    UP_RIGHT_LEFT,
    DOWN_LEFT,
    UP_DOWN_LEFT,
    RIGHT_DOWN_LEFT,
    UP_RIGHT_DOWN_LEFT
  };
  
  constexpr DirBits operator&(const DirBits left, const DirBits right) {
    return static_cast<DirBits>(static_cast<DirType>(left) & static_cast<DirType>(right));
  }
  constexpr DirBits operator|(const DirBits left, const DirBits right) {
    return static_cast<DirBits>(static_cast<DirType>(left) | static_cast<DirType>(right));
  }
  constexpr DirBits operator^(const DirBits left, const DirBits right) {
    return static_cast<DirBits>(static_cast<DirType>(left) ^ static_cast<DirType>(right));
  }
  constexpr DirBits operator~(const DirBits bits) {
    return static_cast<DirBits>(~static_cast<DirType>(bits));
  }
  
  constexpr DirBits &operator&=(DirBits &left, const DirBits right) {
    return left = (left & right);
  }
  constexpr DirBits &operator|=(DirBits &left, const DirBits right) {
    return left = (left | right);
  }
  constexpr DirBits &operator^=(DirBits &left, const DirBits right) {
    return left = (left ^ right);
  }
  
  ///Get the opposite set of directions
  constexpr DirBits opposite(const DirBits bits) {
    return static_cast<DirBits>(static_cast<DirType>(bits) ^ DirType(0b1111));
  }
  
  ///Rotate a set of directions clockwise
  constexpr DirBits rotateCW(const DirBits bits, DirType count = 1) {
    //rotate the 4 least significant bits to the left
    count = count & DirType(0b11);
    const DirType shifted = static_cast<DirType>(bits) << count;
    return static_cast<DirBits>(
      (shifted & DirType(0b1111))
      | ((shifted & DirType(0b11110000)) >> 4)
    );
  }
  
  ///Rotate a set of directions counter-clockwise (anti-clockwise)
  constexpr DirBits rotateCCW(const DirBits bits, const DirType count = 1) {
    return rotateCW(bits, -count);
  }
  
  ///Does the set have any horizontal directions?
  constexpr bool hasHori(const DirBits bits) {
    return (static_cast<DirType>(bits) & DirType(0b1010)) != DirType(0);
  }
  
  ///Does the set have any vertical directions?
  constexpr bool hasVert(const DirBits bits) {
    return (static_cast<DirType>(bits) & DirType(0b0101)) != DirType(0);
  }
  
  ///Set a direction in the set to true
  constexpr DirBits set(const DirBits bits, const Dir dir) {
    return static_cast<DirBits>(
      static_cast<DirType>(bits) | (DirType(1) << static_cast<DirType>(dir))
    );
  }
  
  ///Set a direction in the set to false
  constexpr DirBits reset(const DirBits bits, const Dir dir) {
    return static_cast<DirBits>(
      static_cast<DirType>(bits) & ~(DirType(1) << static_cast<DirType>(dir))
    );
  }
  
  ///Invert a direction in the set (false -> true, true -> false)
  constexpr DirBits flip(const DirBits bits, const Dir dir) {
    return static_cast<DirBits>(
      static_cast<DirType>(bits) ^ (DirType(1) << static_cast<DirType>(dir))
    );
  }
  
  ///Test if a direction is true
  constexpr bool test(const DirBits bits, const Dir dir) {
    return static_cast<DirType>(bits) & (DirType(1) << static_cast<DirType>(dir));
  }
  
  ///Check if all of the directions are true
  constexpr bool all(const DirBits bits) {
    return bits == DirBits::UP_RIGHT_DOWN_LEFT;
  }
  
  ///Check if any of the directions are true
  constexpr bool any(const DirBits bits) {
    return bits != DirBits::NONE;
  }
  
  ///Check if none of the directions are true
  constexpr bool none(const DirBits bits) {
    return bits == DirBits::NONE;
  }
  
  ///Count the number of directions that are set
  constexpr DirType count(const DirBits bits) {
    DirType bitsInt = static_cast<DirType>(bits);
    DirType sum = bitsInt & DirType(1);
    bitsInt >>= 1;
    sum += bitsInt & DirType(1);
    bitsInt >>= 1;
    sum += bitsInt & DirType(1);
    bitsInt >>= 1;
    sum += bitsInt & DirType(1);
    return sum;
  }
}

#endif
