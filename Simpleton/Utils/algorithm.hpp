//
//  algorithm.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 6/11/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_algorithm_hpp
#define engine_utils_algorithm_hpp

#include <algorithm>

namespace Utils {
  template <typename Container, typename Element>
  bool contains(const Container &c, const Element &e) {
    return std::find(std::cbegin(c), std::cend(c), e) != std::cend(c);
  }
  
  template <typename ContainerA, typename ContainerB, typename Equal = std::equal_to<>>
  bool equal(const ContainerA &a, const ContainerB &b, Equal &&comp = {}) {
    return std::equal(std::cbegin(a), std::cend(a), std::cbegin(b), std::forward<Equal>(comp));
  }
  
  template <typename ContainerA, typename ContainerB, typename Equal = std::equal_to<>>
  bool equal_size(const ContainerA &a, const ContainerB &b, Equal &&comp = {}) {
    return std::size(a) == std::size(b) && Utils::equal(a, b, std::forward<Equal>(comp));
  }
  
  template <typename Container, typename Less = std::less<>>
  void sort(Container &c, Less &&comp = {}) {
    return std::sort(std::begin(c), std::end(c), std::forward<Less>(comp));
  }
  
  template <typename Container, typename Equal = std::equal_to<>>
  auto adjacent_find(const Container &c, Equal &&comp = {}) {
    return std::adjacent_find(std::cbegin(c), std::cend(c), std::forward<Equal>(comp));
  }
}

#endif
