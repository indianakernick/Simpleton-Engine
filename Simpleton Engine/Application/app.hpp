//
//  app.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 9/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_application_app_hpp
#define engine_application_app_hpp

#include "../Time/delta.hpp"

namespace Game {
  class App {
  public:
    App() = default;
    virtual ~App() = default;
    
    template <typename DurationType>
    void mainLoop() {
      bool running = init();
      Time::Delta<DurationType> deltaTime;
      while (running) {
        const uint64_t delta = deltaTime.get();
        running = running && input(delta);
        running = running && update(delta);
        render(delta);
      }
      quit();
    }

  protected:
    static constexpr bool RUN = true;
    static constexpr bool QUIT = false;
    
  private:
    virtual bool init() = 0;
    virtual void quit() = 0;
    virtual bool input(uint64_t) = 0;
    virtual bool update(uint64_t) = 0;
    virtual void render(uint64_t) = 0;
  };
}

#endif
