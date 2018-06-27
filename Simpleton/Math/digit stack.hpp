//
//  digit stack.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 29/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_math_digit_stack_hpp
#define engine_math_digit_stack_hpp

#include <cmath>
#include <type_traits>

namespace Math {
  /*
  A fast alternative to this
  
  void bad() {
    std::string digits;
    digits.push_back('1');
    digits.push_back('2');
    digits.push_back('3');
    int number;
    std::from_chars(digits.data(), digits.data() + digits.size(), number);
  }
  
  void good() {
    Math::DigitStack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    int number = stack.get();
  }
  */

  class Negative {};
  constexpr Negative NEGATIVE;

  template <typename Value_, int BASE_ = 10>
  class DigitStack {
  public:
    using Value = Value_;
    static constexpr Value BASE = static_cast<Value>(BASE_);
    
    static_assert(BASE > Value(1));

    DigitStack() = default;
    explicit DigitStack(Negative)
      : neg(true) {}
    explicit DigitStack(const Value val)
      : val(val) {
      if (val == Value(0)) {
        numDigits = 1;
        return;
      } else if (val < Value(0)) {
        neg = true;
      }
      numDigits = std::log2(neg ? -val : val) / std::log2(BASE) + 1.0;
    }
    
    void push(const Value digit) {
      if (neg) {
        val = val * BASE - digit;
      } else {
        val = val * BASE + digit;
      }
      ++numDigits;
    }
    void pop() {
      if (numDigits != 0) {
        val /= BASE;
        --numDigits;
      }
    }
    void clear(const bool negative = false) {
      val = 0;
      numDigits = 0;
      neg = negative;
    }
    
    Value get() const {
      return val;
    }
    bool empty() const {
      return numDigits == 0;
    }
    unsigned char size() const {
      return numDigits;
    }
    
  private:
    Value val = 0;
    unsigned char numDigits = 0;
    bool neg = false;
  };
}

#endif
