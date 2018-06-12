//
//  numeric iterators.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 12/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_numeric_iterators_hpp
#define engine_utils_numeric_iterators_hpp

#include <iterator>

namespace Utils {
  template <typename Number_, Number_ Delta = 1>
  class AddingIterator {
  public:
    using difference_type = Number_;
    using value_type = Number_;
    using reference = const value_type &;
    using pointer = const value_type *;
    using iterator_category = std::random_access_iterator_tag;
    
    explicit AddingIterator(const value_type value)
      : value(value) {}
    
    bool operator==(const AddingIterator other) const {
      return value == other.value;
    }
    bool operator!=(const AddingIterator other) const {
      return value != other.value;
    }
    
    AddingIterator &operator+=(const difference_type advance) {
      value += advance * Delta;
    }
    AddingIterator &operator-=(const difference_type advance) {
      value -= advance * Delta;
    }
    
    AddingIterator operator+(const difference_type advance) const {
      return {value + advance * Delta};
    }
    AddingIterator operator-(const difference_type advance) const {
      return {value - advance * Delta};
    }
    
    difference_type operator-(const AddingIterator other) const {
      return value - other.value;
    }
    
    AddingIterator &operator++() {
      value += Delta;
      return *this;
    }
    AddingIterator &operator--() {
      value -= Delta;
      return *this;
    }
    AddingIterator operator++(int) {
      const AddingIterator copy = *this;
      value += Delta;
      return *this;
    }
    AddingIterator operator--(int) {
      const AddingIterator copy = *this;
      value -= Delta;
      return *this;
    }
    
    value_type operator*() const {
      return value;
    }
    
  private:
    value_type value;
  };
  
  template <typename Iter>
  class Range {
  public:
    constexpr Range(const Iter begin, const Iter end)
      : begin_{begin}, end_{end} {}
  
    constexpr Iter begin() const noexcept {
      return begin_;
    }
    constexpr Iter end() const noexcept {
      return end_;
    }
  
  private:
    Iter begin_;
    Iter end_;
  };
  
  template <typename Number>
  constexpr auto numRange(const Number begin, const Number end) {
    using Iterator = AddingIterator<Number>;
    return Range<Iterator>{
      Iterator{begin}, Iterator{end}
    };
  }
  
  template <typename Number>
  constexpr auto numRangeRev(const Number begin, const Number end) {
    using Iterator = AddingIterator<Number, Number{0} - Number{1}>;
    return Range<Iterator>{
      Iterator{end - Number{1}}, Iterator{begin - Number{1}}
    };
  }
}

#endif
