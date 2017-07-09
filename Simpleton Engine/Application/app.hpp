//
//  app.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 9/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_application_app_hpp
#define engine_application_app_hpp

#include "../Utils/member function.hpp"
#include "../Time/main loop.hpp"

namespace Game {
  class App {
  public:
    App() = default;
    virtual ~App() = default;
    
    template <typename Duration>
    void mainLoop(const uint64_t step, const uint32_t maxSteps) {
      if (!init()) {
        return;
      }
      
      Time::Mainloop<Duration>::fixedWithVarPrePost(
        Utils::memFunWrap(this, &App::input),
        Utils::memFunWrap(this, &App::update),
        Utils::memFunWrap(this, &App::render),
        step,
        maxSteps
      );
      
      quit();
    }
    
  private:
    virtual bool init() = 0;
    virtual void quit() = 0;
    virtual bool input(uint64_t) = 0;
    virtual bool update(uint64_t) = 0;
    virtual void render(uint64_t) = 0;
  };
}

#endif
