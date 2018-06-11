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

namespace Sprite::detail {
  inline void from_json(const json &j, Sprite::Anim &anim) {
    anim = {
      j.at("sprite").get<ID>(),
      j.at("frames").get<ID>()
    };
  }
  
  inline void from_json(const json &j, Sprite::DelayAnim &anim) {
    anim = {
      j.at("sprite").get<ID>(),
      j.at("frames").get<ID>(),
    };
    auto iter = j.find("delay");
    if (iter != j.cend()) {
      anim.delay(iter->get<ID>());
    }
  }
}

#endif
