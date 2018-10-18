//
//  buffer.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 25/09/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_buffer_hpp
#define engine_memory_buffer_hpp

#include "alloc.hpp"

namespace Memory {
  class Buffer {
  public:
    explicit Buffer(size_t) noexcept;
    Buffer(size_t, std::byte) noexcept;
    
    Buffer(Buffer &&) noexcept;
    Buffer &operator=(Buffer &&) noexcept;
    ~Buffer() noexcept;
    
    bool operator==(const Buffer &) const noexcept;
    bool operator!=(const Buffer &) const noexcept;
    bool operator<(const Buffer &) const noexcept;
    bool operator>(const Buffer &) const noexcept;
    bool operator<=(const Buffer &) const noexcept;
    bool operator>=(const Buffer &) const noexcept;
    
    void swap(Buffer &) noexcept;
    void copyFrom(const Buffer &) noexcept;
    
    Buffer dup() const noexcept;
    Buffer dup(size_t) const noexcept;
    Buffer dup(size_t, std::byte) const noexcept;
    
    std::byte *findPtr(std::byte) noexcept;
    const std::byte *findPtr(std::byte) const noexcept;
    size_t findIdx(std::byte) const noexcept;
    
    template <typename T = std::byte>
    T *data() noexcept {
      return reinterpret_cast<T *>(mData);
    }
    template <typename T = std::byte>
    const T *data() const noexcept {
      return reinterpret_cast<const T *>(mData);
    }
    template <typename T = std::byte>
    const T *cdata() const noexcept {
      return reinterpret_cast<const T *>(mData);
    }
    
    template <typename T = size_t>
    T size() const noexcept {
      return static_cast<T>(mSize);
    }
    
    template <typename T = std::byte>
    T *begin() noexcept {
      return reinterpret_cast<T *>(mData);
    }
    template <typename T = std::byte>
    T *end() noexcept {
      return reinterpret_cast<T *>(mData + mSize);
    }
    
    template <typename T = std::byte>
    const T *begin() const noexcept {
      return reinterpret_cast<const T *>(mData);
    }
    template <typename T = std::byte>
    const T *end() const noexcept {
      return reinterpret_cast<const T *>(mData + mSize);
    }
    
    template <typename T = std::byte>
    const T *cbegin() const noexcept {
      return reinterpret_cast<const T *>(mData);
    }
    template <typename T = std::byte>
    const T *cend() const noexcept {
      return reinterpret_cast<const T *>(mData + mSize);
    }
  
  private:
    std::byte *mData;
    size_t mSize;
  };
  
  inline void swap(Buffer &, Buffer &) noexcept;
}

#include "buffer.inl"

#endif
