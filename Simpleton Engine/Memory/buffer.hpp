//
//  buffer.hpp
//  Game Engine
//
//  Created by Indi Kernick on 25/09/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_raw_hpp
#define engine_memory_raw_hpp

#include <cstdint>
#include <cassert>
#include <cstring>
#include <memory>
#include "byteptr.hpp"
#include <algorithm>
#include <functional>

namespace Memory {
  class Buffer;
}

template <>
struct std::hash<Memory::Buffer> {
  size_t operator()(const Memory::Buffer &) const;
};

namespace Memory {
  Byte *alloc(size_t);
  void free(Byte *);
  ///Does not free the pointer, simply does nothing
  void nofree(Byte *);

  class Zero {};
  class One {};
  
  constexpr Zero ZERO_INIT {};
  constexpr One ONE_INIT {};

  class Buffer {
  friend size_t std::hash<Buffer>::operator()(const Buffer &) const;
  public:
    explicit Buffer(size_t);
    Buffer(size_t, Zero);
    Buffer(size_t, One);
    Buffer(size_t, Byte);
    Buffer(void *, size_t, const std::function<void (Byte *)> & = &free);
    Buffer(const Buffer &) = default;
    Buffer(Buffer &&) = default;
    ~Buffer() = default;
    
    Buffer &operator=(const Buffer &) = default;
    Buffer &operator=(Buffer &&) = default;
    
    bool operator==(const Buffer &) const;
    bool operator!=(const Buffer &) const;
    bool operator<(const Buffer &) const;
    
    ///Both buffers refer to same memory
    bool sameMemory(const Buffer &) const;
    
    void swap(Buffer &);
    
    template <typename T = Byte>
    inline T *data() {
      return reinterpret_cast<T *>(mData.get());
    }
    template <typename T = Byte>
    inline const T *data() const {
      return reinterpret_cast<const T *>(mData.get());
    }
    template <typename T = Byte>
    inline const T *cdata() const {
      return reinterpret_cast<const T *>(mData.get());
    }
    
    template <typename T = size_t>
    inline T size() const {
      return static_cast<T>(mSize);
    }
    
    inline Byte *operator+(size_t i) {
      assert(i < mSize);
      return begin() + i;
    }
    inline const Byte *operator+(size_t i) const {
      assert(i < mSize);
      return begin() + i;
    }
    
    template <typename T = Byte>
    inline size_t index(const T *p) const {
      assert(p >= begin());
      assert(p < end());
      
      return p - begin();
    }
    
    inline size_t index(const void *p) const {
      return index<Byte>(reinterpret_cast<const Byte *>(p));
    }
    
    template <typename T = Byte>
    inline T *begin() {
      return reinterpret_cast<T *>(mData.get());
    }
    template <typename T = Byte>
    inline T *end() {
      return reinterpret_cast<T *>(mData.get() + mSize);
    }
    
    template <typename T = Byte>
    inline const T *begin() const {
      return reinterpret_cast<const T *>(mData.get());
    }
    template <typename T = Byte>
    inline const T *end() const {
      return reinterpret_cast<const T *>(mData.get() + mSize);
    }
    
    template <typename T = Byte>
    inline const T *cbegin() const {
      return reinterpret_cast<const T *>(mData.get());
    }
    template <typename T = Byte>
    inline const T *cend() const {
      return reinterpret_cast<const T *>(mData.get() + mSize);
    }
  
  private:
    std::shared_ptr<Byte> mData;
    size_t mSize;
  };
}

#endif
