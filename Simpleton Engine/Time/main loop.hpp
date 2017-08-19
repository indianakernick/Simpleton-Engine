//
//  main loop.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 9/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_time_main_loop_hpp
#define engine_time_main_loop_hpp

#include <cmath>
#include <thread>
#include "get.hpp"
#include "../Utils/function traits.hpp"

namespace Time {
  template <typename Duration>
  struct Mainloop {
  private:
    using Rep = typename Duration::rep;
    
  public:
    ///Call an update method with a time delta
    template <typename Function>
    static bool update(Function &&func, const Rep delta) {
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
    
    ///Variable time step with no synchonization
    template <typename Function>
    static void varNoSync(Function &&func) {
      Point<Duration> last = getPoint<Duration>();
      bool ok = true;
     
      while (ok) {
        const Point<Duration> now = get<Duration>();
        const Rep delta = (now - last).count();
        last = now;
        
        ok = update(func, delta);
      }
    }
    
    ///Fixed time step with synchonization
    template <typename Function>
    static void fixedSync(Function &&func, const Rep step) {
      bool ok = true;
      
      while (ok) {
        const Point<Duration> start = getPoint<Duration>();
        
        ok = update(func, step);
        
        const Duration elapsed = getPoint<Duration>() - start;
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
      const Rep step,
      const uint32_t maxSteps
    ) {
      Rep lag = 0;
      Point<Duration> last = getPoint<Duration>();
      const Rep maxStep = step * maxSteps;
      bool ok = true;
      
      while (ok) {
        const Point<Duration> now = getPoint<Duration>();
        const Duration elapsed = now - last;
        last = now;
        lag += elapsed.count();
        
        const Rep actualStep = [lag, step, maxStep]() {
          if constexpr (std::is_floating_point<Rep>::value) {
            return std::min(lag - std::fmod(lag, step), maxStep);
          } else {
            return std::min(lag - lag % step, maxStep);
          }
        }();
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
