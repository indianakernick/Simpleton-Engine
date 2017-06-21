//
//  implicit unique ptr.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 20/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_implicit_unique_ptr_hpp
#define engine_utils_implicit_unique_ptr_hpp

#include <utility>

namespace Utils {
  template <typename Type, void (*DELETER)(Type *)>
  class ImplicitUniquePtr {
  public:
    //not explicit
    ImplicitUniquePtr(Type *ptr)
      : ptr(ptr) {}
    ImplicitUniquePtr(const Type *ptr)
      : ptr(ptr) {}
    ImplicitUniquePtr(const ImplicitUniquePtr &) = delete;
    ImplicitUniquePtr(ImplicitUniquePtr &&other)
      : ptr(std::exchange(other.ptr, nullptr)) {}
  
    ImplicitUniquePtr &operator=(const ImplicitUniquePtr &) = delete;
    ImplicitUniquePtr &operator=(ImplicitUniquePtr &&other) {
      ptr = std::exchange(other.ptr, nullptr);
    }
    ~ImplicitUniquePtr() {
      DELETER(ptr);
    }
    
    //this is what makes this different from std::unique_ptr
    operator Type *() {
      return ptr;
    }
    operator const Type *() const {
      return ptr;
    }
    
    operator bool() const {
      return ptr;
    }
    
    Type &operator*() {
      return *ptr;
    }
    const Type &operator*() const {
      return *ptr;
    }
    
    Type *operator->() {
      return ptr;
    }
    const Type *operator->() const {
      return ptr;
    }
    
    Type *release() {
      return std::exchange(ptr, nullptr);
    }
    
    void reset() {
      DELETER(ptr);
      ptr = nullptr;
    }
    
    void swap(ImplicitUniquePtr &other) {
      std::swap(ptr, other.ptr);
    }
    
    Type *get() {
      return ptr;
    }
    const Type *get() const {
      return ptr;
    }
  
  protected:
    Type *ptr;
  };
}

#endif
