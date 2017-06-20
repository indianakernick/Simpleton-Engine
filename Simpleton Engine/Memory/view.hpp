//
//  view.hpp
//  Game Engine
//
//  Created by Indi Kernick on 26/09/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_memory_view_hpp
#define engine_memory_view_hpp

#include "buffer.hpp"

namespace Memory {
  template <typename T>
  std::enable_if_t<std::is_pod<T>::value, void>
  free(T *data) {
    operator delete(data);
  }
  
  template <typename T>
  std::enable_if_t<std::is_pod<T>::value, void>
  nofree(T *) {}

  ///A Primitive type memory view. For types that don't need their
  ///constructors and destructors called
  template <typename T>
  class View {
  
  static_assert(std::is_pod<T>::value, "Memory::View can only handle POD types. Use std::vector<> for non-POD types");
  
  public:
    explicit View(const Buffer &buf)
      : buf(buf) {
      assert(buf.size() % sizeof(T) == 0);
    }
    explicit View(Buffer &&buf)
      : buf(buf) {
      assert(buf.size() % sizeof(T) == 0);
    }
    explicit View(size_t size)
      : buf(size * sizeof(T)) {}
    View(size_t size, Zero zero)
      : buf(size * sizeof(T), zero) {}
    View(size_t size, One one)
      : buf(size * sizeof(T), one) {}
    View(T *data, size_t size, const std::function<void (T *)> &deleter = &free<T>)
      : buf(data, size * sizeof(T), [deleter] (Byte *data) {
          deleter(reinterpret_cast<T *>(data));
        }) {
      assert(deleter);
    }
    
    View(const View<T> &) = default;
    View(View<T> &&) = default;
    ~View() = default;
    
    View<T> &operator=(const View<T> &) = default;
    View<T> &operator=(View<T> &&) = default;
    
    bool operator==(const View<T> &other) const {
      return buf == other.buf;
    }
    bool operator!=(const View<T> &other) const {
      return buf != other.buf;
    }
    bool operator<(const View<T> &other) const {
      return buf < other.buf;
    }
    
    template <typename U>
    bool sameMemory(const View<U> &other) {
      return buf.sameMemory(other.buf);
    }
    
    template <typename U>
    void swap(View<U> &other) {
      assert(other.buf.size() % sizeof(T) == 0);
      assert(buf.size() % sizeof(U) == 0);
      buf.swap(other.buf);
    }
    
    inline T &operator[](size_t i) {
      return *(begin() + i);
    }
    inline const T &operator[](size_t i) const {
      return *(begin() + i);
    }
    
    inline T *data() {
      return buf.data<T>();
    }
    inline const T *data() const {
      return buf.data<T>();
    }
    
    template <typename U = size_t>
    inline U size() const {
      return buf.size<U>() / sizeof(T);
    }
    
    inline T *operator+(size_t i) {
      assert(i < buf.size() / sizeof(T));
      return buf.begin<T>() + i;
    }
    inline const T *operator+(size_t i) const {
      assert(i < buf.size() / sizeof(T));
      return buf.begin<T>() + i;
    }
    
    inline T *begin() {
      return buf.begin<T>();
    }
    inline T *end() {
      return buf.end<T>();
    }
    
    inline const T *begin() const {
      return buf.begin<T>();
    }
    inline const T *end() const {
      return buf.end<T>();
    }
    
    inline const T *cbegin() const {
      return buf.cbegin<T>();
    }
    inline const T *cend() const {
      return buf.cend<T>();
    }
    
    Buffer buf;
  };
  
  using StringView = View<char>;
  using WStringView = View<wchar_t>;
  
  using Uint8View  = View< uint8_t>;
  using Int8View   = View<  int8_t>;
  using Uint16View = View<uint16_t>;
  using Int16View  = View< int16_t>;
  using Uint32View = View<uint32_t>;
  using Int32View  = View< int32_t>;
  using Uint64View = View<uint64_t>;
  using Int64View  = View< int64_t>;
  
  using Float32View  = View<float>;
  using Float64View  = View<double>;
  using Float128View = View<long double>;
}

#endif
