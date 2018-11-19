//
//  algorithm.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 6/11/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_algorithm_hpp
#define engine_utils_algorithm_hpp

#include <cassert>
#include <algorithm>

namespace Utils {
  namespace detail {
    template <typename Container, typename Element>
    auto find(const Container &c, const Element &e, int) -> decltype(c.find(e)) {
      return c.find(e);
    }
    
    template <typename Container, typename Element>
    auto find(const Container &c, const Element &e, long) {
      return std::find(std::begin(c), std::end(c), e);
    }
  }
  
  /// Return an iterator to an element in a sequence
  template <typename Container, typename Element>
  auto find(Container &c, const Element &e) {
    return detail::find(c, e);
  }

  /// Determine whether an element exists in a sequence
  template <typename Container, typename Element>
  bool contains(const Container &c, const Element &e) {
    return find(c, e) != std::end(c);
  }
  
  /// Compare two sequences. Assumes they are the same size
  template <typename ContainerA, typename ContainerB, typename Equal = std::equal_to<>>
  bool equal(const ContainerA &a, const ContainerB &b, Equal &&comp = {}) {
    assert(std::size(a) == std::size(b));
    return std::equal(std::cbegin(a), std::cend(a), std::cbegin(b), std::forward<Equal>(comp));
  }
  
  /// Compare two sequences. Checks their size
  template <typename ContainerA, typename ContainerB, typename Equal = std::equal_to<>>
  bool equal_size(const ContainerA &a, const ContainerB &b, Equal &&comp = {}) {
    return std::size(a) == std::size(b) && Utils::equal(a, b, std::forward<Equal>(comp));
  }
  
  /// Sort a sequence
  template <typename Container, typename Less = std::less<>>
  void sort(Container &c, Less &&comp = {}) {
    return std::sort(std::begin(c), std::end(c), std::forward<Less>(comp));
  }
  
  /// Find the first adjacent duplicate in a sequence
  template <typename Container, typename Equal = std::equal_to<>>
  auto adjacent_find(const Container &c, Equal &&comp = {}) {
    return std::adjacent_find(std::cbegin(c), std::cend(c), std::forward<Equal>(comp));
  }
}

#endif
