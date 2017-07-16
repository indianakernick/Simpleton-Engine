//
//  type list.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 16/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef type_list_hpp
#define type_list_hpp

#include <cstddef>

namespace Utils {
  template <typename ...Types>
  struct TypeList {};
  
  using EmptyList = TypeList<>;
  
  template <typename List>
  struct IsEmptyHelper {
    static constexpr bool value = false;
  };
  
  template <>
  struct IsEmptyHelper<EmptyList> {
    static constexpr bool value = true;
  };
  
  template <typename List>
  constexpr bool listIsEmpty = IsEmptyHelper<List>::value;
  
  template <typename List>
  struct SizeHelper;
  
  template <typename ...Types>
  struct SizeHelper<TypeList<Types...>> {
    static constexpr size_t value = sizeof...(Types);
  };
  
  template <typename List>
  constexpr size_t listSize = SizeHelper<List>::value;
  
  template <typename List, typename Type>
  struct ContainsHelper;
  
  template <typename Type, typename ...Types>
  struct ContainsHelper<TypeList<Type, Types...>, Type> {
    static constexpr bool value = true;
  };
  
  template <typename Type, typename FirstType, typename ...Types>
  struct ContainsHelper<TypeList<FirstType, Types...>, Type> {
    static constexpr bool value = ContainsHelper<TypeList<Types...>, Type>::value;
  };
  
  template <typename Type>
  struct ContainsHelper<EmptyList, Type> {
    static constexpr bool value = false;
  };
  
  template <typename List, typename Type>
  constexpr bool listContains = ContainsHelper<List, Type>::value;
  
  template <typename List, size_t INDEX>
  struct TypeAtIndexHelper;
  
  template <size_t INDEX, typename FirstType, typename ...Types>
  struct TypeAtIndexHelper<TypeList<FirstType, Types...>, INDEX> {
    using type = typename TypeAtIndexHelper<TypeList<Types...>, INDEX - 1>::type;
  };
  
  template <typename FirstType, typename ...Types>
  struct TypeAtIndexHelper<TypeList<FirstType, Types...>, 0> {
    using type = FirstType;
  };
  
  class NoType {};
  
  template <size_t INDEX>
  struct TypeAtIndexHelper<EmptyList, INDEX> {
    using type = NoType;
  };
  
  template <typename List, size_t INDEX>
  using typeAtIndex = typename TypeAtIndexHelper<List, INDEX>::type;
  
  template <typename List, typename Type, size_t INDEX = 0>
  struct IndexOfHelper;
  
  template <typename Type, typename ...Types, size_t INDEX>
  struct IndexOfHelper<TypeList<Type, Types...>, Type, INDEX> {
    static constexpr size_t value = INDEX;
  };
  
  template <typename Type, typename FirstType, typename ...Types, size_t INDEX>
  struct IndexOfHelper<TypeList<FirstType, Types...>, Type, INDEX> {
    static constexpr size_t value = IndexOfHelper<TypeList<Types...>, Type, INDEX + 1>::value;
  };
  
  template <typename Type, size_t INDEX>
  struct IndexOfHelper<EmptyList, Type, INDEX> {
    static constexpr size_t value = -size_t(1);
  };
  
  template <typename List, typename Type>
  constexpr size_t indexOf = IndexOfHelper<List, Type>::value;
  
  template <typename List, template <typename> typename Operation>
  struct TransformHelper;
  
  template <template <typename> typename Operation, typename ...Types>
  struct TransformHelper<TypeList<Types...>, Operation> {
    using type = TypeList<typename Operation<Types>::type...>;
  };
  
  template <typename List, template <typename> typename Operation>
  using transformList = typename TransformHelper<List, Operation>::type;
  
  template <typename LeftList, typename RightList>
  struct ConcatHelper;
  
  template <typename ...LeftTypes, typename ...RightTypes>
  struct ConcatHelper<TypeList<LeftTypes...>, TypeList<RightTypes...>> {
    using type = TypeList<LeftTypes..., RightTypes...>;
  };
  
  template <typename LeftList, typename RightList>
  using concatLists = typename ConcatHelper<LeftList, RightList>::type;
  
  template <typename A, typename B>
  struct is_same {
    static constexpr bool value = false;
  };
  
  template <typename T>
  struct is_same<T, T> {
    static constexpr bool value = true;
  };
  
  template <typename T>
  struct add_pointer {
    using type = T *;
  };
  
  static_assert(listIsEmpty<TypeList<>>);
  static_assert(!listIsEmpty<TypeList<int>>);

  static_assert(listSize<TypeList<int, char, long>> == 3);
  static_assert(listSize<TypeList<int>> == 1);
  static_assert(listSize<TypeList<>> == 0);

  static_assert(listContains<TypeList<int, char, long>, int>);
  static_assert(listContains<TypeList<int, char, long>, char>);
  static_assert(listContains<TypeList<int, char, long>, long>);
  static_assert(!listContains<TypeList<int, char, long>, float>);

  static_assert(is_same<typeAtIndex<TypeList<int, char, long>, 0>, int>::value);
  static_assert(is_same<typeAtIndex<TypeList<int, char, long>, 1>, char>::value);
  static_assert(is_same<typeAtIndex<TypeList<int, char, long>, 2>, long>::value);
  static_assert(is_same<typeAtIndex<TypeList<int, char, long>, 3>, NoType>::value);

  static_assert(indexOf<TypeList<int, char, long>,  int> == 0);
  static_assert(indexOf<TypeList<int, char, long>,  char> == 1);
  static_assert(indexOf<TypeList<int, char, long>,  long> == 2);
  static_assert(indexOf<TypeList<int, char, long>,  float> == -size_t(1));
  
  static_assert(is_same<transformList<TypeList<int, char, long>, add_pointer>, TypeList<int *, char *, long *>>::value);
  static_assert(is_same<transformList<EmptyList, add_pointer>, EmptyList>::value);
  
  static_assert(is_same<concatLists<TypeList<int, char>, TypeList<long>>, TypeList<int, char, long>>::value);
  static_assert(is_same<concatLists<TypeList<int, char, long>, EmptyList>, TypeList<int, char, long>>::value);
  static_assert(is_same<concatLists<EmptyList, EmptyList>, EmptyList>::value);
}

#endif
