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
  
    DeltaAnim() = default;
    explicit DeltaAnim(uint64_t);
    DeltaAnim(uint64_t, EdgeMode, Dir = Dir::FORWARD);
    
    void advance(uint64_t);
    
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
    Dir getDir() const;
    
    void setDuration(uint64_t);
    uint64_t getDuration() const;
    
    void setEdgeMode(EdgeMode);
    EdgeMode getEdgeMode() const;
    
    bool isRunning() const;
    bool isStopped() const;
    bool isPaused() const;
    
    void setProgress(double);
    double getProgress() const;
    
  private:
    enum class State {
      RUNNING,
      STOPPED,
      PAUSED
    };
  
    uint64_t duration = 0;
    uint64_t progressTime = 0;
    EdgeMode edgeMode = EdgeMode::REPEAT;
    Dir dir = Dir::FORWARD;
    State state = State::STOPPED;
    
    void handleEdges();
  };
};

#endif
