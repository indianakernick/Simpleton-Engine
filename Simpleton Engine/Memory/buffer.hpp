//
//  buffer.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 25/09/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_buffer_hpp
#define engine_memory_buffer_hpp

#include <memory>
#include <cassert>
#include <cstdint>
#include "byteptr.hpp"

namespace Memory {
  class Zero {};
  class One {};
  class NoDelete {};
  
  constexpr Zero ZERO_INIT {};
  constexpr One ONE_INIT {};
  constexpr NoDelete NO_DELETE {};

  class Buffer {
  public:
    explicit Buffer(size_t);
    Buffer(size_t, Zero);
    Buffer(size_t, One);
    Buffer(size_t, Byte);
    Buffer(void *, size_t);
    Buffer(void *, size_t, NoDelete);
    
    Buffer(Buffer &&) = default;
    Buffer &operator=(Buffer &&) = default;
    
    bool operator==(const Buffer &) const;
    bool operator!=(const Buffer &) const;
    bool operator<(const Buffer &) const;
    
    void swap(Buffer &);
    void copyFrom(const Buffer &);
    
    template <typename T = Byte>
    T *data() {
      return reinterpret_cast<T *>(mData.get());
    }
    template <typename T = Byte>
    const T *data() const {
      return reinterpret_cast<const T *>(mData.get());
    }
    template <typename T = Byte>
    const T *cdata() const {
      return reinterpret_cast<const T *>(mData.get());
    }
    
    template <typename T = size_t>
    T size() const {
      return static_cast<T>(mSize);
    }
    
    template <typename T = Byte>
    T *begin() {
      return reinterpret_cast<T *>(mData.get());
    }
    template <typename T = Byte>
    T *end() {
      return reinterpret_cast<T *>(mData.get() + mSize);
    }
    
    template <typename T = Byte>
    const T *begin() const {
      return reinterpret_cast<const T *>(mData.get());
    }
    template <typename T = Byte>
    const T *end() const {
      return reinterpret_cast<const T *>(mData.get() + mSize);
    }
    
    template <typename T = Byte>
    const T *cbegin() const {
      return reinterpret_cast<const T *>(mData.get());
    }
    template <typename T = Byte>
    const T *cend() const {
      return reinterpret_cast<const T *>(mData.get() + mSize);
    }
  
  private:
    std::unique_ptr<Byte, void (*)(void *)> mData;
    size_t mSize;
  };
}

#include "buffer.inl"

#endif
