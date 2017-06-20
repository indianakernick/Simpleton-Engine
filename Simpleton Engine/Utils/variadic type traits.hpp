//
//  variadic type traits.hpp
//  Game Engine
//
//  Created by Indi Kernick on 19/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_variadic_type_traits_hpp
#define engine_utils_variadic_type_traits_hpp

#include <type_traits>

//finally found a use for template templates!

namespace Utils {
  ///All of the arguments have the trait
  template <template <typename ARG> typename TRAIT,
            typename ...ARGS>
  constexpr bool all = (TRAIT<ARGS>::value && ...);

  ///Any of the arguments have the trait
  template <template <typename ARG> typename TRAIT,
            typename ...ARGS>
  constexpr bool any = (TRAIT<ARGS>::value || ...);

  ///None of the arguments have the trait
  template <template <typename ARG> typename TRAIT,
            typename ...ARGS>
  constexpr bool none = (!TRAIT<ARGS>::value && ...);

  //i would have liked to have a variable number of traits but that doesn't compile

  ///All of the arguments have both of the traits
  template <template <typename ARG> typename TRAIT0,
            template <typename ARG> typename TRAIT1,
            typename ...ARGS>
  constexpr bool allBoth = ((TRAIT0<ARGS>::value && TRAIT1<ARGS>::value) && ...);

  ///Any of the arguments have both of the traits
  template <template <typename ARG> typename TRAIT0,
            template <typename ARG> typename TRAIT1,
            typename ...ARGS>
  constexpr bool anyBoth = ((TRAIT0<ARGS>::value && TRAIT1<ARGS>::value) || ...);

  ///None of the arguments have both of the traits
  template <template <typename ARG> typename TRAIT0,
            template <typename ARG> typename TRAIT1,
            typename ...ARGS>
  constexpr bool noneBoth = (!(TRAIT0<ARGS>::value && TRAIT1<ARGS>::value) && ...);


  ///All of the arguments have either of the traits
  template <template <typename ARG> typename TRAIT0,
            template <typename ARG> typename TRAIT1,
            typename ...ARGS>
  constexpr bool allEither = ((TRAIT0<ARGS>::value || TRAIT1<ARGS>::value) && ...);

  ///Any of the arguments have either of the traits
  template <template <typename ARG> typename TRAIT0,
            template <typename ARG> typename TRAIT1,
            typename ...ARGS>
  constexpr bool anyEither = ((TRAIT0<ARGS>::value || TRAIT1<ARGS>::value) || ...);

  ///None of the arguments have either of the traits
  template <template <typename ARG> typename TRAIT0,
            template <typename ARG> typename TRAIT1,
            typename ...ARGS>
  constexpr bool noneEither = (!(TRAIT0<ARGS>::value || TRAIT1<ARGS>::value) && ...);


  ///All of the arguments have either of the traits
  template <template <typename ARG> typename TRAIT0,
            template <typename ARG> typename TRAIT1,
            typename ...ARGS>
  constexpr bool allNeither = ((!TRAIT0<ARGS>::value && !TRAIT1<ARGS>::value) && ...);

  ///Any of the arguments have either of the traits
  template <template <typename ARG> typename TRAIT0,
            template <typename ARG> typename TRAIT1,
            typename ...ARGS>
  constexpr bool anyNeither = ((!TRAIT0<ARGS>::value && !TRAIT1<ARGS>::value) || ...);

  ///None of the arguments have either of the traits
  template <template <typename ARG> typename TRAIT0,
            template <typename ARG> typename TRAIT1,
            typename ...ARGS>
  constexpr bool noneNeither = (!(!TRAIT0<ARGS>::value && !TRAIT1<ARGS>::value) && ...);


  ///All of the arguments have the binary trait given the left trait argument
  template <template <typename ARG0, typename ARG1> typename TRAIT,
            typename LEFT,
            typename ...ARGS>
  constexpr bool allLeft = (TRAIT<LEFT, ARGS>::value && ...);

  ///Any of the arguments have the binary trait given the left trait argument
  template <template <typename ARG0, typename ARG1> typename TRAIT,
            typename LEFT,
            typename ...ARGS>
  constexpr bool anyLeft = (TRAIT<LEFT, ARGS>::value || ...);

  ///None of the arguments have the binary trait given the left trait argument
  template <template <typename ARG0, typename ARG1> typename TRAIT,
            typename LEFT,
            typename ...ARGS>
  constexpr bool noneLeft = (!TRAIT<LEFT, ARGS>::value && ...);


  ///All of the arguments have the binary trait given the right trait argument
  template <template <typename ARG0, typename ARG1> typename TRAIT,
            typename RIGHT,
            typename ...ARGS>
  constexpr bool allRight = (TRAIT<ARGS, RIGHT>::value && ...);

  ///Any of the arguments have the binary trait given the right trait argument
  template <template <typename ARG0, typename ARG1> typename TRAIT,
            typename RIGHT,
            typename ...ARGS>
  constexpr bool anyRight = (TRAIT<ARGS, RIGHT>::value || ...);

  ///None of the arguments have the binary trait given the right trait argument
  template <template <typename ARG0, typename ARG1> typename TRAIT,
            typename RIGHT,
            typename ...ARGS>
  constexpr bool noneRight = (!TRAIT<ARGS, RIGHT>::value && ...);


  ///All of the arguments are the same
  template <typename ...ARGS>
  constexpr bool allSame = allLeft<std::is_same, ARGS...>;

  ///All of the arguments are the same as the first argument
  template <typename ARG, typename ...ARGS>
  constexpr bool allSameAs = allLeft<std::is_same, ARG, ARGS...>;

  ///Any of the arguments are the same as the first argument
  template <typename ARG, typename ...ARGS>
  constexpr bool anySameAs = anyLeft<std::is_same, ARG, ARGS...>;

  ///None of the arguments are the same as the first argument
  template <typename ARG, typename ...ARGS>
constexpr bool noneSameAs = noneLeft<std::is_same, ARG, ARGS...>;
}

#endif
