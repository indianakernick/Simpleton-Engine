//
//  json.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 10/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_sprite_json_hpp
#define engine_sprite_json_hpp

#include "anim.hpp"
#include "../Data/json.hpp"

namespace Sprite {
  inline void from_json(const json &j, Anim &anim) {
    anim = {
      j.at("sprite").get<ID>(),
      j.at("frames").get<ID>()
    };
  }
  
  inline void from_json(const json &j, DelayAnim &anim) {
    const auto iter = j.find("delay");
    anim = {
      j.at("sprite").get<ID>(),
      j.at("frames").get<ID>(),
      iter == j.cend() ? 1 : iter->get<ID>()
    };
  }
}

#endif
