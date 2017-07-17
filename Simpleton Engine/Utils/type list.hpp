//
//  type list.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 16/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_type_list_hpp
#define engine_utils_type_list_hpp

#include <tuple>
#include <cstddef>
#include <utility>
#include <type_traits>
#include "type name.hpp"

namespace Utils {
  template <typename ...Types>
  struct TypeList {};
  
  //Packing
  
  template <typename List, template <typename ...> typename TemplateType>
  struct UnpackIntoHelper;
  
  template <template <typename ...> typename TemplateType, typename ...Types>
  struct UnpackIntoHelper<TypeList<Types...>, TemplateType> {
    using type = TemplateType<Types...>;
  };
  
  template <typename List, template <typename ...> typename TemplateType>
  using UnpackInto = typename UnpackIntoHelper<List, TemplateType>::type;
  
  template <typename TemplateType>
  struct PackFromHelper;
  
  template <template <typename ...> typename TemplateType, typename ...Types>
  struct PackFromHelper<TemplateType<Types...>> {
    using type = TypeList<Types...>;
  };
  
  template <typename TemplateType>
  using PackFrom = typename PackFromHelper<TemplateType>::type;
  
  template <typename List>
  using ListToTuple = UnpackInto<List, std::tuple>;
  
  template <typename Tuple>
  using ListFromTuple = PackFrom<Tuple>;
  
  static_assert(std::is_same<ListToTuple<TypeList<int, char, long>>, std::tuple<int, char, long>>::value);
  static_assert(std::is_same<ListFromTuple<std::tuple<int, char, long>>, TypeList<int, char, long>>::value);
  
  //Empty
  
  using EmptyList = TypeList<>;
  
  template <typename List>
  constexpr bool listIsEmpty = std::is_same<List, EmptyList>::value;
  
  static_assert(listIsEmpty<TypeList<>>);
  static_assert(!listIsEmpty<TypeList<int>>);
  
  //Push front
  
  template <typename List, typename Type, size_t NUM>
  struct PushFrontHelper;

  template <typename Type, size_t NUM, typename ...Types>
  struct PushFrontHelper<TypeList<Types...>, Type, NUM> {
    using type = typename PushFrontHelper<TypeList<Type, Types...>, Type, NUM - 1>::type;
  };

  template <typename Type, typename ...Types>
  struct PushFrontHelper<TypeList<Types...>, Type, 1> {
    using type = TypeList<Type, Types...>;
  };
    
  template <typename List, typename Type>
  struct PushFrontHelper<List, Type, 0> {
    using type = List;
  };

  template <typename List, typename Type, size_t NUM>
  using PushFront = typename PushFrontHelper<List, Type, NUM>::type;
  
  static_assert(std::is_same<PushFront<TypeList<char, long>, int, 1>, TypeList<int, char, long>>::value);
  static_assert(std::is_same<PushFront<TypeList<char>, int, 3>, TypeList<int, int, int, char>>::value);
  
  //Pop front
  
  template <typename List, size_t NUM>
  struct PopFrontHelper;
  
  template <size_t NUM, typename First, typename ...Types>
  struct PopFrontHelper<TypeList<First, Types...>, NUM> {
    using type = std::conditional_t<
      NUM == 0,
      TypeList<First, Types...>,
      typename PopFrontHelper<TypeList<Types...>, NUM - 1>::type
    >;
  };
  
  template <size_t NUM>
  struct PopFrontHelper<EmptyList, NUM> {
    using type = EmptyList;
  };
  
  template <typename List, size_t NUM = 1>
  using PopFront = typename PopFrontHelper<List, NUM>::type;
  
  static_assert(std::is_same<PopFront<TypeList<int, char, long>, 2>, TypeList<long>>::value);
  
  //Size
  
  template <typename List>
  struct SizeHelper;
  
  template <typename ...Types>
  struct SizeHelper<TypeList<Types...>> {
    static constexpr size_t value = sizeof...(Types);
  };
  
  template <typename List>
  constexpr size_t listSize = SizeHelper<List>::value;
  
  static_assert(listSize<TypeList<int, char, long>> == 3);
  static_assert(listSize<TypeList<int>> == 1);
  static_assert(listSize<TypeList<>> == 0);
  
  //Contains
  
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
  
  static_assert(listContains<TypeList<int, char, long>, int>);
  static_assert(listContains<TypeList<int, char, long>, char>);
  static_assert(listContains<TypeList<int, char, long>, long>);
  static_assert(!listContains<TypeList<int, char, long>, float>);
  
  //At index
  
  template <typename List, size_t INDEX>
  struct AtIndexHelper;
  
  template <size_t INDEX, typename FirstType, typename ...Types>
  struct AtIndexHelper<TypeList<FirstType, Types...>, INDEX> {
    using type = typename AtIndexHelper<
      TypeList<Types...>, INDEX - 1
    >::type;
  };
  
  template <typename First, typename ...Types>
  struct AtIndexHelper<TypeList<First, Types...>, 0> {
    using type = First;
  };
  
  class NoType {};
  
  template <size_t INDEX>
  struct AtIndexHelper<EmptyList, INDEX> {
    using type = NoType;
  };
  
  template <typename List, size_t INDEX>
  using AtIndex = typename AtIndexHelper<List, INDEX>::type;
  
  template <typename List, size_t INDEX>
  using AtIndexRev = typename AtIndexHelper<List, listSize<List> - INDEX - 1>::type;
  
  template <typename List>
  using FirstType = AtIndex<List, 0>;
  
  template <typename List>
  using LastType = AtIndexRev<List, 0>;
  
  static_assert(std::is_same<AtIndex<TypeList<int, char, long>, 0>, int>::value);
  static_assert(std::is_same<AtIndex<TypeList<int, char, long>, 1>, char>::value);
  static_assert(std::is_same<AtIndex<TypeList<int, char, long>, 2>, long>::value);
  static_assert(std::is_same<AtIndex<TypeList<int, char, long>, 3>, NoType>::value);
  
  //Index of
  
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
  
  static_assert(indexOf<TypeList<int, char, long>,  int> == 0);
  static_assert(indexOf<TypeList<int, char, long>,  char> == 1);
  static_assert(indexOf<TypeList<int, char, long>,  long> == 2);
  static_assert(indexOf<TypeList<int, char, long>,  float> == -size_t(1));
  
  //Transform
  
  template <typename List, template <typename> typename Operation>
  struct TransformHelper;
  
  template <template <typename> typename Operation, typename ...Types>
  struct TransformHelper<TypeList<Types...>, Operation> {
    using type = TypeList<typename Operation<Types>::type...>;
  };
  
  template <typename List, template <typename> typename Operation>
  using TransformList = typename TransformHelper<List, Operation>::type;
  
  static_assert(std::is_same<TransformList<TypeList<int, char, long>, std::add_pointer>, TypeList<int *, char *, long *>>::value);
  static_assert(std::is_same<TransformList<EmptyList, std::add_pointer>, EmptyList>::value);
  
  //Concat
  
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
  using ConcatLists = typename ConcatHelper<Lists...>::type;
  
  static_assert(std::is_same<ConcatLists<TypeList<int, char>, TypeList<long>>, TypeList<int, char, long>>::value);
  static_assert(std::is_same<ConcatLists<TypeList<int, char, long>, EmptyList>, TypeList<int, char, long>>::value);
  static_assert(std::is_same<ConcatLists<EmptyList, EmptyList>, EmptyList>::value);
  
  //Repeat
  
  template <typename Type, size_t SIZE>
  using RepeatType = PushFront<EmptyList, Type, SIZE>;
  
  static_assert(std::is_same<RepeatType<int, 3>, TypeList<int, int, int>>::value);
  
  //Reverse
  
  template <typename List, typename Sequence>
  struct ReverseHelperHelper;
  
  template <typename List, size_t ...INDICIES>
  struct ReverseHelperHelper<List, std::index_sequence<INDICIES...>> {
    using type = TypeList<AtIndexRev<List, INDICIES>...>;
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
  using ReverseList = typename ReverseHelper<List>::type;
  
  static_assert(std::is_same<ReverseList<TypeList<int, char, long>>, TypeList<long, char, int>>::value);
  
  //Merge
  
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
      ConcatLists<TypeList<LeftFirst>, typename Merge<TypeList<LeftTypes...>, TypeList<RightFirst, RightTypes...>, Less>::type>,
      ConcatLists<TypeList<RightFirst>, typename Merge<TypeList<LeftFirst, LeftTypes...>, TypeList<RightTypes...>, Less>::type>
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
  
  template <typename Left, typename Right>
  struct SizeLess {
    static constexpr bool value = sizeof(Left) < sizeof(Right);
  };
  
  template <typename Left, typename Right>
  struct HashLess {
    static constexpr bool value = typeHash<Left>() < typeHash<Right>();
  };
  
  static_assert(std::is_same<
    typename Merge<TypeList<char>, TypeList<int, long>, SizeLess>::type,
    TypeList<char, int, long>
  >::value);
  
  //Take Front
  
  template <typename List, size_t SIZE>
  struct TakeFrontHelper {
    using type = ConcatLists<
      TypeList<FirstType<List>>,
      typename TakeFrontHelper<PopFront<List>, SIZE - 1>::type
    >;
  };
  
  template <typename List>
  struct TakeFrontHelper<List, 0> {
    using type = EmptyList;
  };
  
  template <size_t SIZE>
  struct TakeFrontHelper<EmptyList, SIZE> {
    using type = EmptyList;
  };
  
  template <>
  struct TakeFrontHelper<EmptyList, 0> {
    using type = EmptyList;
  };
  
  template <typename List, size_t SIZE>
  using TakeFront = typename TakeFrontHelper<List, SIZE>::type;
  
  static_assert(
    std::is_same<
      TypeList<int, char>,
      TakeFront<TypeList<int, char, long>, 2>
    >::value
  );
  
  //Splice
  
  template <typename List, size_t POS, size_t SIZE>
  using SpliceList = TakeFront<PopFront<List, POS>, SIZE>;
  
  static_assert(std::is_same<FirstType<TypeList<int, char, long>>, int>::value);
  static_assert(std::is_same<TakeFront<TypeList<int, char, long>, 1>, TypeList<int>>::value);
  
  template <typename List>
  using LeftHalf = SpliceList<List, 0, listSize<List> / 2>;
  
  template <typename List>
  using RightHalf = SpliceList<List, listSize<List> / 2, (listSize<List> + 1) / 2>;
  
  static_assert(std::is_same<LeftHalf<TypeList<int, char, long>>, TypeList<int>>::value);
  static_assert(std::is_same<RightHalf<TypeList<int, char, long>>, TypeList<char, long>>::value);
  
  //Sort
  
  template <typename List, template <typename, typename> typename Less>
  struct SortHelper {
    using type = typename Merge<
      typename SortHelper<LeftHalf<List>, Less>::type,
      typename SortHelper<RightHalf<List>, Less>::type,
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
  using SortList = typename SortHelper<List, Less>::type;
  
  static_assert(std::is_same<SortList<TypeList<int, char, long>, SizeLess>, TypeList<char, int, long>>::value);
  static_assert(std::is_same<SortList<EmptyList, SizeLess>, EmptyList>::value);
  
  //Filter
  
  template <typename List, template <typename> typename Pred>
  struct FilterHelper;
  
  template <template <typename> typename Pred, typename First, typename ...Types>
  struct FilterHelper<TypeList<First, Types...>, Pred> {
    using type = std::conditional_t<
      Pred<First>::value,
      ConcatLists<TypeList<First>, typename FilterHelper<TypeList<Types...>, Pred>::type>,
      typename FilterHelper<TypeList<Types...>, Pred>::type
    >;
  };
  
  template <template <typename> typename Pred>
  struct FilterHelper<EmptyList, Pred> {
    using type = EmptyList;
  };
  
  template <typename List, template <typename> typename Pred>
  using FilterList = typename FilterHelper<List, Pred>::type;
  
  static_assert(std::is_same<FilterList<TypeList<int, char, float, long, double>, std::is_integral>, TypeList<int, char, long>>::value);
  
  //Is permutation of
  
  template <typename First, typename Second>
  constexpr bool isPermutOf = std::is_same<SortList<First, HashLess>, SortList<Second, HashLess>>::value;
  
  static_assert(isPermutOf<TypeList<int>, TypeList<int>>);
  static_assert(isPermutOf<TypeList<int, char, long>, TypeList<char, int, long>>);
  static_assert(isPermutOf<EmptyList, EmptyList>);
  static_assert(!isPermutOf<TypeList<int, char, long>, TypeList<char, int, int>>);
  
  //For each
  
  template <typename T>
  struct Type {
    using type = T;
  };
  
  template <typename List>
  struct ForEachHelper;
  
  template <typename ...Types>
  struct ForEachHelper<TypeList<Types...>> {
    template <typename Function>
    static void iter(Function &&function) {
      (function(Type<Types>()), ...);
    }
  };
  
  template <typename List, typename Function>
  void forEach(Function &&func) {
    ForEachHelper<List>::iter(std::forward<Function>(func));
  }
  
  template <typename Tuple, typename Function, size_t ...INDICIES>
  static void forEachTupleHelper(Tuple &&tuple, Function &&function, std::index_sequence<INDICIES...>) {
    (function(std::get<INDICIES>(tuple)), ...);
  }
  
  template <typename Tuple, typename Function>
  void forEach(Tuple &&tuple, Function &&function) {
    forEachTupleHelper(
      std::forward<Tuple>(tuple),
      std::forward<Function>(function),
      std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>()
    );
  }
}

#endif
