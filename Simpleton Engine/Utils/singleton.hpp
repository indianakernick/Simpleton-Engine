//
//  singleton.hpp
//  Game Engine
//
//  Created by Indi Kernick on 8/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_singleton_hpp
#define engine_utils_singleton_hpp

#include <memory>
#include <cassert>
#include "instance limiter.hpp"

namespace Utils {
  template <typename Type>
  class Singleton : ForceSingleton<Type> {
  protected:
    Singleton() = default;
    Singleton(const Singleton &) = delete;
    Singleton(Singleton &&) = default;
    ~Singleton() {
      static_assert(std::is_base_of<Singleton, Type>::value);
    };
    
    Singleton &operator=(const Singleton &) = delete;
    Singleton &operator=(Singleton &&) = delete;

  public:
    template <typename ...Args>
    static std::enable_if_t<
      std::is_constructible<Type, Args...>::value
    >
    create(Args &&... args) {
      assert(instance == nullptr);
      instance = std::make_unique<Type>(std::forward<Args>(args)...);
    }
    
    static Type &get() {
      assert(instance != nullptr);
      return *instance;
    }
    
    static Type *getPtr() {
      return instance.get();
    }
    
    static void destroy() {
      instance.reset();
    }
    
  private:
    static std::unique_ptr<Type> instance;
  };

  template<typename Type>
  std::unique_ptr<Type> Singleton<Type>::instance = nullptr;
}

#endif
