//
//  atom.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 7/9/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_atom_hpp
#define engine_utils_atom_hpp

#include <array>
#include <cstdint>

namespace Utils {
  using AtomInt = uint64_t;
  
  template <size_t Bits>
  struct Atom {
    static constexpr size_t bits = Bits;
    static constexpr size_t digits = 64 / bits;
    static constexpr AtomInt digit_mask = (1 << Bits) - 1;
    using char_array = std::array<char, digits + 1>;
  
    constexpr explicit Atom(const AtomInt value)
      : value{value} {}
    
    constexpr operator AtomInt() const noexcept {
      return value;
    }
    
  private:
    AtomInt value;
  };
  
  using Atom5 = Atom<5>;
  using Atom6 = Atom<6>;

  namespace detail {
    constexpr AtomInt no = ~AtomInt{};
    
    template <size_t Bits>
    struct Tables;
    
    template <>
    struct Tables<5> {
      static constexpr AtomInt to[128] {
    //  32 special characters
        0,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
       no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
    //     !  "  #  $  %  &  '  (  )  *  +  ,  -  .  /
       no,no,no,27,no,no,no,no,no,no,no,no,no,no,28,29,
    //  0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?
       no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
    //  @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O
       30, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,
    //  P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _
       16,17,18,19,20,21,22,23,24,25,26,no,no,no,no,31,
    //  `  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o
       no, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,
    //  p  q  r  s  t  u  v  w  x  y  z  {  |  }  ~  DEL
       16,17,18,19,20,21,22,23,24,25,26,no,no,no,no,no,
      };
      
      static constexpr char from[(1 << 5) + 1] {"\0abcdefghijklmnopqrstuvwxyz#./@_"};
    };
    
    template <>
    struct Tables<6> {
      static constexpr AtomInt to[128] {
    //  32 special characters
        0,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
       no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
    //     !  "  #  $  %  &  '  (  )  *  +  ,  -  .  /
       no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
    //  0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?
       53,54,55,56,57,58,59,60,61,62,no,no,no,no,no,no,
    //  @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O
       no, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,
    //  P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _
       16,17,18,19,20,21,22,23,24,25,26,no,no,no,no,63,
    //  `  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o
       no,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
    //  p  q  r  s  t  u  v  w  x  y  z  {  |  }  ~  DEL
       42,43,44,45,46,47,48,49,50,51,52,no,no,no,no,no,
      };
      static constexpr char from[(1 << 6) + 1] {"\0ABCDEFGHIJKLMnoPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_"};
    };
  }

  template <size_t Bits>
  constexpr Atom<Bits> toAtom(const char *data, const size_t size) {
    AtomInt set = 0;
    if (size > Atom<Bits>::digits) {
      throw "String too long";
    }
    for (AtomInt index = 0; index != size; ++index) {
      const size_t tableIndex = data[index];
      const AtomInt code = detail::Tables<Bits>::to[tableIndex];
      if (code == detail::no) {
        throw "Invalid character in string";
      }
      set |= code << (index * Bits);
    }
    return Atom<Bits>{set};
  }

  template <size_t Bits>
  constexpr typename Atom<Bits>::char_array fromAtom(const Atom<Bits> atom) {
    typename Atom<Bits>::char_array array;
    array.fill(0);
    AtomInt set = atom;
    for (AtomInt index = 0; index != Atom<Bits>::digits; ++index) {
      const AtomInt code = (set >> (index * Bits)) & Atom<Bits>::digit_mask;
      array[index] = detail::Tables<Bits>::from[code];
    }
    return array;
  }
  
  namespace Literals {
    constexpr Atom5 operator""_atom5(const char *data, const size_t size) noexcept {
      return toAtom<5>(data, size);
    }
    
    constexpr Atom6 operator""_atom6(const char *data, const size_t size) noexcept {
      return toAtom<6>(data, size);
    }
    
    static_assert(""_atom5 == 0);
    static_assert("A"_atom5 == 1);
    static_assert("AA"_atom5 == 33);
    static_assert("ABCDEFGHIJKL"_atom5 == 445092485129178177);
    
    static_assert(""_atom6 == 0);
    static_assert("A"_atom6 == 1);
    static_assert("AA"_atom6 == 65);
    static_assert("ABCDEFGHIJ"_atom6 == 182712931821039745);
  }
}

#endif
