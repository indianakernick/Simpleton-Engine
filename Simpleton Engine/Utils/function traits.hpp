//
//  function traits.hpp
//  Game Engine
//
//  Created by Indi Kernick on 4/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_function_traits_hpp
#define engine_utils_function_traits_hpp

#include <tuple>
#include <functional>

namespace Utils {
  template <typename Function>
  struct function_traits;

  template <typename Function>
  using function_ret = typename function_traits<Function>::ret;

  template <typename Function, size_t I>
  using function_arg = typename function_traits<Function>::template arg<I>;

  template <typename Function>
  constexpr bool is_member_variable = function_traits<Function>::is_member && function_traits<Function>::mem::is_var;

  template <typename Function>
  constexpr bool is_member_function = function_traits<Function>::is_member && function_traits<Function>::mem::is_fun;

  //function
  template <typename Return, typename ...Args>
  struct function_traits<Return (Args...)> {
    using ret = Return;
    
    static constexpr size_t arity = sizeof...(Args);
    
    using args = std::tuple<Args...>;
    
    template <size_t I>
    using arg = typename std::tuple_element<I, args>::type;
    
    static constexpr bool is_member = false;
  };

  template <typename Function, typename Class, bool CONST, bool LVALUE, bool RVALUE>
  struct member_function : function_traits<Function> {
    static constexpr bool is_member = true;
    
    struct mem {
      using class_type = Class;
      
      static constexpr bool is_var = false;
      static constexpr bool is_fun = true;
      
      struct fun {
        static constexpr bool is_const = CONST;
        static constexpr bool is_lvalue = LVALUE;
        static constexpr bool is_rvalue = RVALUE;
      };
    };
  };

  template <typename Function, typename Class>
  struct member_variable : function_traits<Function> {
    static constexpr bool is_member = true;
    
    struct mem {
      using class_type = Class;
      
      static constexpr bool is_var = true;
      static constexpr bool is_fun = false;
      
      struct var {
        using type = typename function_traits<Function>::ret;
      };
    };
  };

  //pointer to function
  template <typename Return, typename ...Args>
  struct function_traits<Return (*)(Args...)> :
    function_traits<Return (Args...)> {};

  //pointer to member function
  template <typename Class, typename Return, typename ...Args>
  struct function_traits<Return (Class::*) (Args...)> :
    member_function<Return (Args...), Class, false, false, false> {};

  //pointer to const member function
  template <typename Class, typename Return, typename ...Args>
  struct function_traits<Return (Class::*) (Args...) const> :
    member_function<Return (Args...), Class, true, false, false> {};

  //pointer to lvalue member function
  template <typename Class, typename Return, typename ...Args>
  struct function_traits<Return (Class::*) (Args...) &> :
    member_function<Return (Args...), Class, false, true, false> {};

  //pointer to const lvalue member function
  template <typename Class, typename Return, typename ...Args>
  struct function_traits<Return (Class::*) (Args...) const &> :
    member_function<Return (Args...), Class, true, true, false> {};

  //pointer to rvalue member function
  template <typename Class, typename Return, typename ...Args>
  struct function_traits<Return (Class::*) (Args...) &&> :
    member_function<Return (Args...), Class, false, false, true> {};

  //pointer to const rvalue member function
  template <typename Class, typename Return, typename ...Args>
  struct function_traits<Return (Class::*) (Args...) const &&> :
    member_function<Return (Args...), Class, true, false, true> {};

  //pointer to member variable
  template <typename Class, typename Type>
  struct function_traits<Type (Class::*)> :
    member_variable<Type (), Class> {};

  //std::function
  template <typename Return, typename ...Args>
  struct function_traits<std::function<Return (Args...)>> :
    function_traits<Return (Args...)> {};

  //functor
  template <typename Functor>
  struct function_traits :
    //use member function pointer specialization
    function_traits<decltype(&Functor::operator())> {};

  //const function
  template <typename Function>
  struct function_traits<const Function> : function_traits<Function> {};

  //lvalue reference to function
  template <typename Function>
  struct function_traits<Function &> : function_traits<Function> {};

  //rvalue reference to function
  template <typename Function>
struct function_traits<Function &&> : function_traits<Function> {};
}

#endif
