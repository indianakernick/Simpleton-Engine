//
//  main.cpp
//  Pack
//
//  Created by Indi Kernick on 24/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include <vector>
#include <cassert>
#include <iostream>

template <typename Float>
bool test(const size_t divisor) {
  assert(divisor != 0);
  
  std::cout << "Divisor: " << divisor << '\n';
  
  std::vector<Float> floats;
  floats.reserve(divisor + 1);
  
  for (size_t i = 0; i <= divisor; ++i) {
    floats.push_back(static_cast<Float>(i) / divisor);
  }
  
  for (size_t i = 0; i <= divisor; ++i) {
    if (i != static_cast<size_t>(floats[i] * divisor)) {
      std::cout
        << i
        << " / "
        << divisor
        << '\t'
        << (floats[i] * divisor)
        << '\t'
        << static_cast<size_t>(floats[i] * divisor)
        << '\n'
      ;
      return false;
    }
  }

  return true;
}

int main(int argc, const char * argv[]) {
  
  std::cout << (16777217.0f == 16777216.0f) << '\n';
  
  std::cout.precision(10);
  size_t divisor = 1;
  //while (test<float>(divisor)) {
  //  divisor <<= 1;
 // }
  
  return 0;
}
