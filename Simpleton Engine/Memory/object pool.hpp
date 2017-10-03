//
//  object pool.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 3/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_object_pool_hpp
#define engine_memory_object_pool_hpp

#include <new>
#include <vector>
#include "buffer.hpp"

namespace Memory {
  template <typename Obj, bool POD = std::is_pod<Obj>::value>
  class ObjectPool;

  ///POD
  template <typename Obj>
  class ObjectPool<Obj, true> {
  public:
    using Object = Obj;
    
    ObjectPool(const size_t numObjects)
      : buf(numObjects * sizeof(Object)) {}
    ObjectPool(const size_t numObjects, const Object obj)
      : buf(numObjects * sizeof(Object)) {
      std::uninitialized_fill(buf.begin<Object>(), buf.end<Object>(), obj);
    }
    ~ObjectPool() = default;
    
    Object &operator[](const size_t i) {
      return *(buf.data<Object>() + i);
    }
    const Object &operator[](const size_t i) const {
      return *(buf.data<Object>() + i);
    }
    
    Object *begin() {
      return buf.begin<Object>();
    }
    const Object *cbegin() const {
      return buf.cbegin<Object>();
    }
    Object *end() {
      return buf.end<Object>();
    }
    const Object *cend() const {
      return buf.cend<Object>();
    }
    
    Buffer buf;
  };

  ///Non-POD
  template <typename Obj>
  class ObjectPool<Obj, false> {
  public:
    using Object = Obj;
    
    ObjectPool(const size_t numObjects)
      : buf(numObjects * sizeof(Object)), constructed(numObjects, false) {}
    ObjectPool(const size_t numObjects, const Object obj)
      : buf(numObjects * sizeof(Object)), constructed(numObjects, true) {
      std::uninitialized_fill(buf.begin<Object>(), buf.end<Object>(), obj);
    }
    ~ObjectPool() {
      for (size_t o = 0; o != buf.size(); ++o) {
        if (constructed[o]) {
          (buf.data<Object>() + o)->~Object();
        }
      }
    }
    
    Object &operator[](const size_t i) {
      return *(buf.data<Object>() + i);
    }
    const Object &operator[](const size_t i) const {
      return *(buf.data<Object>() + i);
    }
    
    Object *begin() {
      return buf.begin<Object>();
    }
    const Object *cbegin() const {
      return buf.cbegin<Object>();
    }
    Object *end() {
      return buf.end<Object>();
    }
    const Object *cend() const {
      return buf.cend<Object>();
    }
    
    template <typename ...Args>
    void construct(const size_t i, Args &&... args) {
      if (constructed[i]) {
        std::runtime_error("Tried to construct object where object was already constructed");
      }
      new (buf.data<Object>() + i) Object(std::forward<Args>(args)...);
      constructed[i] = true;
    }
    
    void destroy(const size_t i) {
      if (!constructed[i]) {
        std::runtime_error("Tried to destroy object where no object was constructed");
      }
      (buf.data<Object>() + i)->~Object();
      constructed[i] = false;
    }
    
  private:
    Buffer buf;
    std::vector<bool> constructed;
  };
}

#endif
