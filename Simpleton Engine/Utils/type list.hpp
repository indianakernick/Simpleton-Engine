//
//  type list.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 16/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef type_list_hpp
#define type_list_hpp

#include <tuple>
#include <cstddef>
#include <utility>
#include <type_traits>
#include "type name.hpp"

namespace Utils {
  template <typename ...Types>
  struct TypeList {};
  
  template <typename Tuple>
  struct ListFromTupleHelper;
  
  template <typename ...Types>
  struct ListFromTupleHelper<std::tuple<Types...>> {
    using type = TypeList<Types...>;
  };
  
  template <typename Tuple>
  using listFromTuple = typename ListFromTupleHelper<Tuple>::type;
  
  template <typename List>
  struct ListToTupleHelper;
  
  template <typename ...Types>
  struct ListToTupleHelper<TypeList<Types...>> {
    using type = std::tuple<Types...>;
  };
  
  template <typename List>
  using listToTuple = typename ListToTupleHelper<List>::type;
  
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
  
  template <typename List, size_t INDEX>
  using typeAtIndexRev = typename TypeAtIndexHelper<List, listSize<List> - INDEX - 1>::type;
  
  template <typename List>
  using firstType = typeAtIndex<List, 0>;
  
  template <typename List>
  using lastType = typeAtIndexRev<List, 0>;
  
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
  
  template <typename ...Lists>
  struct ConcatHelper;
  
  template <>
  struct ConcatHelper<> {
    using type = EmptyList;
  };
  
  template <typename ...Types>
  struct ConcatHelper<TypeList<Types...>> {
    using type = TypeList<Types...>;
  };
  
  template <typename ...LeftTypes, typename ...RightTypes>
  struct ConcatHelper<TypeList<LeftTypes...>, TypeList<RightTypes...>> {
    using type = TypeList<LeftTypes..., RightTypes...>;
  };
  
  template <typename FirstList, typename ...Lists>
  struct ConcatHelper<FirstList, Lists...> {
    using type = typename ConcatHelper<FirstList, typename ConcatHelper<Lists...>::type>::type;
  };
  
  template <typename ...Lists>
  using concatLists = typename ConcatHelper<Lists...>::type;
  
  template <typename Type, size_t SIZE>
  struct RepeatHelper {
    using type = concatLists<TypeList<Type>, typename RepeatHelper<Type, SIZE - 1>::type>;
  };
  
  template <typename Type>
  struct RepeatHelper<Type, 0> {
    using type = EmptyList;
  };
  
  template <typename Type, size_t SIZE>
  using repeatType = typename RepeatHelper<Type, SIZE>::type;
  
  template <typename List, typename Sequence>
  struct ReverseHelperHelper;
  
  template <typename List, size_t ...INDICIES>
  struct ReverseHelperHelper<List, std::index_sequence<INDICIES...>> {
    using type = TypeList<typeAtIndexRev<List, INDICIES>...>;
  };
  
  template <typename List>
  struct ReverseHelper {
    using type = typename ReverseHelperHelper<List, std::make_index_sequence<listSize<List>>>::type;
  };
  
  template <>
  struct ReverseHelper<EmptyList> {
    using type = EmptyList;
  };
  
  template <typename List>
  using reverseList = typename ReverseHelper<List>::type;
  
  template <typename LeftList, typename RightList, template <typename, typename> typename Less>
  struct Merge;
  
  template <
    template <typename, typename> typename Less,
    typename LeftFirst,
    typename RightFirst,
    typename ...LeftTypes,
    typename ...RightTypes
  >
  struct Merge<TypeList<LeftFirst, LeftTypes...>, TypeList<RightFirst, RightTypes...>, Less> {
    using type = std::conditional_t<
      Less<LeftFirst, RightFirst>::value,
      concatLists<TypeList<LeftFirst>, typename Merge<TypeList<LeftTypes...>, TypeList<RightFirst, RightTypes...>, Less>::type>,
      concatLists<TypeList<RightFirst>, typename Merge<TypeList<LeftFirst, LeftTypes...>, TypeList<RightTypes...>, Less>::type>
    >;
  };
  
  template <typename LeftList, template <typename, typename> typename Less>
  struct Merge<LeftList, EmptyList, Less> {
    using type = LeftList;
  };
  
  template <typename RightList, template <typename, typename> typename Less>
  struct Merge<EmptyList, RightList, Less> {
    using type = RightList;
  };
  
  template <template <typename, typename> typename Less>
  struct Merge<EmptyList, EmptyList, Less> {
    using type = EmptyList;
  };
  
  template <typename List>
  struct PopFrontHelper;
  
  template <typename First, typename ...Types>
  struct PopFrontHelper<TypeList<First, Types...>> {
    using type = TypeList<Types...>;
  };
  
  template <>
  struct PopFrontHelper<EmptyList> {
    using type = EmptyList;
  };
  
  template <typename List>
  using popFront = typename PopFrontHelper<List>::type;
  
  template <typename List, size_t SIZE>
  struct SpliceHelperHelper {
    using type = std::conditional_t<
      SIZE == 0,
      EmptyList,
      concatLists<
        TypeList<firstType<List>>,
        typename SpliceHelperHelper<popFront<List>, SIZE - 1>::type
      >
    >;
  };
  
  template <size_t SIZE>
  struct SpliceHelperHelper<EmptyList, SIZE> {
    using type = EmptyList;
  };
  
  template <>
  struct SpliceHelperHelper<EmptyList, 0> {
    using type = EmptyList;
  };
  
  static_assert(
    std::is_same<
      TypeList<int, char>,
      typename SpliceHelperHelper<TypeList<int, char, long>, 2>::type
    >::value
  );
  
  template <typename List, size_t POS, size_t SIZE>
  struct SpliceHelper;
  
  template <size_t POS, size_t SIZE, typename First, typename ...Types>
  struct SpliceHelper<TypeList<First, Types...>, POS, SIZE> {
    using type = typename SpliceHelper<TypeList<Types...>, POS - 1, SIZE>::type;
  };
  
  template <size_t SIZE, typename First, typename ...Types>
  struct SpliceHelper<TypeList<First, Types...>, 0, SIZE> {
    using type = concatLists<TypeList<First>, typename SpliceHelperHelper<TypeList<Types...>, SIZE - 1>::type>;
  };
  
  template <size_t POS, size_t SIZE>
  struct SpliceHelper<EmptyList, POS, SIZE> {
    using type = EmptyList;
  };
  
  template <typename List>
  struct SpliceHelper<List, 0, 0> {
    using type = EmptyList;
  };
  
  template <>
  struct SpliceHelper<EmptyList, 0, 0> {
    using type = EmptyList;
  };
  
  template <typename List, size_t POS, size_t SIZE>
  using spliceList = typename SpliceHelper<List, POS, SIZE>::type;
  
  template <typename List>
  using leftHalf = spliceList<List, 0, listSize<List> / 2>;
  
  template <typename List>
  using rightHalf = spliceList<List, listSize<List> / 2, (listSize<List> + 1) / 2>;
  
  static_assert(std::is_same<leftHalf<TypeList<int, char, long>>, TypeList<int>>::value);
  static_assert(std::is_same<rightHalf<TypeList<int, char, long>>, TypeList<char, long>>::value);
  
  template <typename List, template <typename, typename> typename Less>
  struct SortHelper {
    using type = typename Merge<
      typename SortHelper<leftHalf<List>, Less>::type,
      typename SortHelper<rightHalf<List>, Less>::type,
      Less
    >::type;
  };
  
  template <typename First, typename Second, template <typename, typename> typename Less>
  struct SortHelper<TypeList<First, Second>, Less> {
    using type = std::conditional_t<
      Less<First, Second>::value,
      TypeList<First, Second>,
      TypeList<Second, First>
    >;
  };
  
  template <typename Type, template <typename, typename> typename Less>
  struct SortHelper<TypeList<Type>, Less> {
    using type = TypeList<Type>;
  };
  
  template <template <typename, typename> typename Less>
  struct SortHelper<EmptyList, Less> {
    using type = EmptyList;
  };
  
  template <typename List, template <typename, typename> typename Less>
  using sortList = typename SortHelper<List, Less>::type;
  
  template <typename Left, typename Right>
  struct SizeLess {
    static constexpr bool value = sizeof(Left) < sizeof(Right);
  };
  
  template <typename Left, typename Right>
  struct HashLess {
    static constexpr bool value = typeHash<Left>() < typeHash<Right>();
  };
  
  template <typename List, template <typename> typename Pred>
  struct FilterHelper;
  
  template <template <typename> typename Pred, typename First, typename ...Types>
  struct FilterHelper<TypeList<First, Types...>, Pred> {
    using type = std::conditional_t<
      Pred<First>::value,
      concatLists<TypeList<First>, typename FilterHelper<TypeList<Types...>, Pred>::type>,
      typename FilterHelper<TypeList<Types...>, Pred>::type
    >;
  };
  
  template <template <typename> typename Pred>
  struct FilterHelper<EmptyList, Pred> {
    using type = EmptyList;
  };
  
  template <typename List, template <typename> typename Pred>
  using filterList = typename FilterHelper<List, Pred>::type;
  
  template <typename First, typename Second>
  constexpr bool isPermutOf = std::is_same<sortList<First, HashLess>, sortList<Second, HashLess>>::value;
  
  template <typename T>
  struct Type {
    using type = T;
  };
  
  template <typename List>
  struct ForEachHelper;
  
  template <typename ...Types>
  struct ForEachHelper<TypeList<Types...>> {
    template <typename Function>
    static constexpr void iter(Function &&func) {
      (func(Type<Types>()), ...);
    }
  };
  
  template <typename List, typename Function>
  constexpr void forEach(Function &&func) {
    ForEachHelper<List>::iter(std::forward<Function>(func));
  }
  
  static_assert(listIsEmpty<TypeList<>>);
  static_assert(!listIsEmpty<TypeList<int>>);

  static_assert(listSize<TypeList<int, char, long>> == 3);
  static_assert(listSize<TypeList<int>> == 1);
  static_assert(listSize<TypeList<>> == 0);

  static_assert(listContains<TypeList<int, char, long>, int>);
  static_assert(listContains<TypeList<int, char, long>, char>);
  static_assert(listContains<TypeList<int, char, long>, long>);
  static_assert(!listContains<TypeList<int, char, long>, float>);

  static_assert(std::is_same<typeAtIndex<TypeList<int, char, long>, 0>, int>::value);
  static_assert(std::is_same<typeAtIndex<TypeList<int, char, long>, 1>, char>::value);
  static_assert(std::is_same<typeAtIndex<TypeList<int, char, long>, 2>, long>::value);
  static_assert(std::is_same<typeAtIndex<TypeList<int, char, long>, 3>, NoType>::value);

  static_assert(indexOf<TypeList<int, char, long>,  int> == 0);
  static_assert(indexOf<TypeList<int, char, long>,  char> == 1);
  static_assert(indexOf<TypeList<int, char, long>,  long> == 2);
  static_assert(indexOf<TypeList<int, char, long>,  float> == -size_t(1));
  
  static_assert(std::is_same<transformList<TypeList<int, char, long>, std::add_pointer>, TypeList<int *, char *, long *>>::value);
  static_assert(std::is_same<transformList<EmptyList, std::add_pointer>, EmptyList>::value);
  
  static_assert(std::is_same<concatLists<TypeList<int, char>, TypeList<long>>, TypeList<int, char, long>>::value);
  static_assert(std::is_same<concatLists<TypeList<int, char, long>, EmptyList>, TypeList<int, char, long>>::value);
  static_assert(std::is_same<concatLists<EmptyList, EmptyList>, EmptyList>::value);
  
  static_assert(std::is_same<repeatType<int, 3>, TypeList<int, int, int>>::value);
  
  static_assert(std::is_same<reverseList<TypeList<int, char, long>>, TypeList<long, char, int>>::value);
  
  static_assert(std::is_same<rightHalf<TypeList<int, char, long>>, TypeList<char, long>>::value);
  
  static_assert(std::is_same<
    typename Merge<TypeList<char>, TypeList<int, long>, SizeLess>::type,
    TypeList<char, int, long>
  >::value);
  
  static_assert(std::is_same<sortList<TypeList<int, char, long>, SizeLess>, TypeList<char, int, long>>::value);
  
  static_assert(isPermutOf<TypeList<int>, TypeList<int>>);
  static_assert(isPermutOf<TypeList<int, char, long>, TypeList<char, int, long>>);
  static_assert(isPermutOf<EmptyList, EmptyList>);
  static_assert(!isPermutOf<TypeList<int, char, long>, TypeList<char, int, int>>);
  
  static_assert(std::is_same<filterList<TypeList<int, char, float, long, double>, std::is_integral>, TypeList<int, char, long>>::value);
}

#endif
