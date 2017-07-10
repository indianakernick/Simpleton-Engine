//
//  main loop.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 9/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_time_main_loop_hpp
#define engine_time_main_loop_hpp

#include <thread>
#include "get.hpp"
#include "../Utils/function traits.hpp"

namespace Time {
  template <typename Duration>
  struct Mainloop {
    ///Call an update method with a time delta
    template <typename Function>
    static bool update(Function &&func, const uint64_t delta) {
      if constexpr (std::is_convertible<Utils::function_ret<Function>, bool>::value) {
        if constexpr (Utils::function_arity<Function> == 1) {
          return static_cast<bool>(func(static_cast<Utils::function_arg<Function, 0>>(delta)));
        } else {
          return static_cast<bool>(func());
        }
      } else {
        if constexpr (Utils::function_arity<Function> == 1) {
          func(static_cast<Utils::function_arg<Function, 0>>(delta));
        } else {
          func();
        }
        return true;
      }
    }
    
    ///Variable time step with no synchonisation
    template <typename Function>
    static void varNoSync(Function &&func) {
      uint64_t last = getI<Duration>();
      bool ok = true;
     
      while (ok) {
        const uint64_t now = getI<Duration>();
        const uint64_t delta = now - last;
        last = now;
        
        ok = update(func, delta);
      }
    }
    
    ///Fixed time step with synchonisation
    template <typename Function>
    static void fixedSync(Function &&func, const uint64_t step) {
      bool ok = true;
      
      while (ok) {
        const uint64_t start = getI<Duration>();
        
        ok = update(func, step);
        
        const uint64_t elapsed = getI<Duration>() - start;
        if (elapsed < step) {
          std::this_thread::sleep_for(Duration(step));
        }
      }
    }
    
    ///Fixed time step with variable pre and post (input and rendering)
    template <typename PreFunc, typename UpdateFunc, typename PostFunc>
    static void fixedWithVarPrePost(
      PreFunc &&preFunc,
      UpdateFunc &&updateFunc,
      PostFunc &&postFunc,
      const uint64_t step,
      const uint32_t maxSteps
    ) {
      uint64_t lag = 0;
      uint64_t last = getI<Duration>();
      const uint64_t maxStep = step * maxSteps;
      bool ok = true;
      
      while (ok) {
        const uint64_t now = getI<Duration>();
        const uint64_t elapsed = now - last;
        last = now;
        lag += elapsed;
        
        const uint64_t actualStep = std::min(lag - lag % step, maxStep);
        ok = update(preFunc, actualStep);
        
        uint32_t steps = maxSteps;
        
        while (lag >= step && steps) {
          ok = ok && update(updateFunc, step);
          lag -= step;
          steps--;
        }
        
        ok = ok && update(postFunc, actualStep);
      }
    }
  };
}

#endif
