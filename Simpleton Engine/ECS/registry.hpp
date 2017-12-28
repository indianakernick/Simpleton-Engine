//
//  registry.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 28/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_ecs_registry_hpp
#define engine_ecs_registry_hpp

#include <limits>
#include <cstdint>
#include <entt/entity/registry.hpp>

namespace ECS {
  using EntityID = uint32_t;
  using Registry = entt::Registry<EntityID>;
  constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();
}

#endif
