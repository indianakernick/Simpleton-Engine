//
//  delta anim.hpp
//  Game Engine
//
//  Created by Indi Kernick on 17/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_time_delta_anim_hpp
#define engine_time_delta_anim_hpp

#include <cstdint>
#include <cassert>
#include "../Math/interpolate.hpp"
#include "../Math/clamp.hpp"

namespace Time {
  class DeltaAnim {
  public:
    enum EdgeMode {
      ///continue past the edge
      NOTHING,
      ///return to the beginning and continue
      REPEAT,
      ///pause on the edge
      PAUSE,
      ///reset
      STOP,
      ///"bounce" off the edge
      CHANGE_DIR
    };
    enum Dir {
      FORWARD,
      BACKWARD
    };
  
    DeltaAnim(uint64_t, EdgeMode = EdgeMode::REPEAT);
    
    void update(uint64_t);
    
    ///Start the animation
    void start();
    ///Stop the animation, return the playhead to the beginning and the dir
    ///to FORWARD
    void stop();
    ///Pause the animation to be resumed later
    void pause();
    ///Resume the animation after it was paused
    void resume();
    
    void setDir(Dir);
    void flipDir();
    Dir getDir();
    
    void setDuration(uint64_t);
    uint64_t getDuration();
    
    void setEdgeMode(EdgeMode);
    EdgeMode getEdgeMode();
    
    bool isRunning();
    bool isStopped();
    bool isPaused();
    
    void setProgress(double);
    double getProgress();
    
  private:
    enum State {
      RUNNING,
      STOPPED,
      PAUSED
    };
  
    uint64_t duration;
    uint64_t progressTime = 0;
    EdgeMode edgeMode;
    Dir dir = FORWARD;
    State state = STOPPED;
    
    void handleEdges();
  };
};

#endif
