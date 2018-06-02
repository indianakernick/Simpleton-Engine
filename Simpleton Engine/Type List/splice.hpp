//
//  splice.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 2/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_type_list_splice_hpp
#define engine_type_list_splice_hpp

#include "concat.hpp"

namespace List {
  // Push front
  
  namespace detail {
    template <typename List, typename Type, size_t Num>
    struct PushFrontI;

    template <typename T, size_t Num, typename... Types>
    struct PushFrontI<Type<Types...>, T, Num> {
      using type = typename PushFrontI<Type<T, Types...>, T, Num - 1>::type;
    };

    template <typename T, typename... Types>
    struct PushFrontI<Type<Types...>, T, 1> {
      using type = Type<T, Types...>;
    };
    
    template <typename List, typename Type>
    struct PushFrontI<List, Type, 0> {
      using type = List;
    };
  }

  /// Push one or more copies of a type onto the front of the list
  template <typename List, typename Type, size_t Num = 1>
  using PushFront = typename detail::PushFrontI<List, Type, Num>::type;
  
  static_assert(Same<PushFront<Type<char, long>, int>, Type<int, char, long>>);
  static_assert(Same<PushFront<Type<char>, int, 3>, Type<int, int, int, char>>);
  
  // Pop front
  
  namespace detail {
    template <bool Cond, typename True, typename False>
    struct ConditionalHelper;
    
    template <typename True, typename False>
    struct ConditionalHelper<true, True, False> {
      using type = True;
    };
    
    template <typename True, typename False>
    struct ConditionalHelper<false, True, False> {
      using type = False;
    };
    
    template <bool Cond, typename True, typename False>
    using Conditional = typename ConditionalHelper<Cond, True, False>::type;
  
    template <typename List, size_t Num>
    struct PopFrontI;
    
    template <size_t Num, typename First, typename... Types>
    struct PopFrontI<Type<First, Types...>, Num> {
      using type = Conditional<
        Num == 0,
        Type<First, Types...>,
        typename PopFrontI<Type<Types...>, Num - 1>::type
      >;
    };
    
    template <size_t Num>
    struct PopFrontI<EmptyType, Num> {
      using type = EmptyType;
    };
  }
  
  /// Pop one or more types from the front of the list
  template <typename List, size_t Num = 1>
  using PopFront = typename detail::PopFrontI<List, Num>::type;
  
  static_assert(Same<PopFront<Type<int, char, long>, 0>, Type<int, char, long>>);
  static_assert(Same<PopFront<Type<int, char, long>, 1>, Type<char, long>>);
  static_assert(Same<PopFront<Type<int, char, long>, 2>, Type<long>>);
  static_assert(Same<PopFront<Type<int, char, long>, 3>, EmptyType>);
  static_assert(Same<PopFront<Type<int, char, long>, 4>, EmptyType>);

  // Take Front
  
  namespace detail {
    template <typename List, size_t Size>
    struct TakeFrontI {
      using type = Concat<
        Type<First<List>>,
        typename TakeFrontI<PopFront<List>, Size - 1>::type
      >;
    };
    
    template <typename List>
    struct TakeFrontI<List, 0> {
      using type = EmptyType;
    };
    
    template <size_t Size>
    struct TakeFrontI<EmptyType, Size> {
      using type = EmptyType;
    };
    
    template <>
    struct TakeFrontI<EmptyType, 0> {
      using type = EmptyType;
    };
  }
  
  /// Take a list of types from the from of the list
  template <typename List, size_t Size>
  using TakeFront = typename detail::TakeFrontI<List, Size>::type;
  
  static_assert(Same<TakeFront<Type<int, char, long>, 2>, Type<int, char>>);
  static_assert(Same<TakeFront<Type<int, char, long>, 1>, Type<int>>);
  
  // Splice
  
  /// Take a sublist of types given a position and size
  template <typename List, size_t Pos, size_t Size>
  using Splice = TakeFront<PopFront<List, Pos>, Size>;
  
  static_assert(Same<Splice<Type<char, short, int, long, long long>, 1, 3>, Type<short, int, long>>);
  
  // Repeat
  
  /// Create a list by repeating a type
  template <typename Type, size_t Size>
  using Repeat = PushFront<EmptyType, Type, Size>;
  
  static_assert(Same<Repeat<int, 3>, Type<int, int, int>>);
}

#endif
