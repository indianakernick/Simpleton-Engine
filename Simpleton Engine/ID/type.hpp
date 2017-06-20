//
//  type.hpp
//  Game Engine
//
//  Created by Indi Kernick on 3/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_id_type_hpp
#define engine_id_type_hpp

#include <type_traits>

namespace ID {
  template <typename Int, typename Group>
  class Counter {
  
    static_assert(std::is_integral<Int>::value, "Int must be an integral type");
  
  public:
    Counter() = delete;
    ~Counter() = delete;
  
  //Friend declarations shall not declare partial specializations
  protected:
    static Int counter;
  };
  
  template <typename Int, typename Group>
  Int Counter<Int, Group>::counter = 0;
  
  ///Creates an ID unique to the type within the Group. IDs are counted and only
  ///available at runtime
  template <typename Int, typename T, typename Group = void>
  class TypeCounter : private Counter<Int, Group> {
  
    static_assert(std::is_integral<Int>::value, "Int must be an integral type");
  
  public:
    TypeCounter() = delete;
    ~TypeCounter() = delete;
    
    static Int get() {
      return ID;
    }
    
  private:
    static const Int ID;
  };
  
  template <typename Int, typename T, typename Group>
  const Int TypeCounter<Int, T, Group>::ID = Counter<Int, Group>::counter++;
  
  template <typename T, typename Group>
  class Hasher {
  public:
    Hasher() = delete;
    ~Hasher() = delete;
    
  protected:
    static void dummy() {}
    static constexpr void (*HASH)() = &Hasher::dummy;
  };
  
  ///Creates an ID unique to the type within the Group. IDs are hashes and
  ///available at compile time
  template <typename Int, typename T, typename Group>
  class TypeHasher : private Hasher<T, Group> {
  
    static_assert(std::is_integral<Int>::value, "Int must be an integral type");
  
  public:
    TypeHasher() = delete;
    ~TypeHasher() = delete;
    
    static constexpr Int get() {
      return ID;
    }
    
  private:
    static constexpr Int ID = reinterpret_cast<Int>(Hasher<T, Group>::HASH);
  };
}

#endif
