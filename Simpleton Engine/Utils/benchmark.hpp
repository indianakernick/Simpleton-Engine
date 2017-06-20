//
//  benchmark.hpp
//  Game Engine
//
//  Created by Indi Kernick on 20/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_benchmark_hpp
#define engine_utils_benchmark_hpp

#include <chrono>
#include <iostream>

#define BENCHMARK(name, ...) \
  {\
    const auto START_TIME = std::chrono::high_resolution_clock::now();\
    {\
      __VA_ARGS__\
    }\
    const auto END_TIME = std::chrono::high_resolution_clock::now();\
    std::cout << #name << "  \t" << (END_TIME - START_TIME).count() / 1'000'000.0 << "ms\n";\
  }

#endif
