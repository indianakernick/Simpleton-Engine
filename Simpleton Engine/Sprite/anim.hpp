//
//  anim.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 10/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_sprite_anim_hpp
#define engine_sprite_anim_hpp

#include "id.hpp"

namespace Sprite {
  class Anim {
  public:
    Anim();
    Anim(ID, ID);
    
    void incr();
    void incrRepeat();
    bool incrStop();
    
    ID firstSprite() const;
    ID sprite() const;
    ID sprite(ID) const;
    template <typename T>
    ID sprite(T) const;
    
    ID frame() const;
    float progress() const;
    bool firstFrame() const;
    bool lastFrame() const;
  
  private:
    ID sprite_;
    ID frames_;
    ID frame_;
  };
  
  class DelayAnim {
  public:
    DelayAnim();
    DelayAnim(ID, ID, ID);
    
    void incr();
    void incrRepeat();
    bool incrStop();
    
    void delay(ID);
    void noDelay();
    void maxDelay();
    void speed(double);
    
    ID firstSprite() const;
    ID sprite() const;
    ID sprite(ID) const;
    template <typename T>
    ID sprite(T) const;
    
  private:
    ID sprite_;
    ID frames_;
    ID delay_;
    ID frame_;
  };
}

#include "anim.inl"

#endif
