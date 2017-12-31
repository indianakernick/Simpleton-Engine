//
//  comp inits.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 30/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_ecs_comp_inits_hpp
#define engine_ecs_comp_inits_hpp

#include <memory>
#include "comp init.hpp"
#include "../Utils/tuple.hpp"
#include "../Utils/type list.hpp"

namespace ECS {
  template <typename CompList>
  class CompInits {
  public:
    CompInits() = default;
    
    ///Initializes the given component
    template <typename Comp>
    void init(
      Comp &comp,
      const json &node,
      const EntityIDmap &idMap,
      const EntityID entity
    ) const {
      assert(getInit<Comp>());
      getInit<Comp>()->initialize(comp, node, idMap, entity);
    }
    
    ///Constructs the initializer for the given component
    template <typename CompInit, typename ...Args>
    void construct(Args &&... args) {
      using Comp = typename CompInit::Comp;
      getInit<Comp>() = std::make_unique<CompInit>(std::forward<Args>(args)...);
    }
    
    ///Creates default initializers for components that don't have initializers
    void constructDefaults() {
      Utils::forEach(inits, [] (auto &init) {
        if (!init) {
          using UniquePtr = std::remove_reference_t<decltype(init)>;
          init = std::make_unique<typename UniquePtr::element_type>();
        }
      });
    }
    
    ///Destroys all component initializers
    void destroyAll() {
      Utils::forEach(inits, [] (auto &init) {
        init.reset();
      });
    }
    
  private:
    template <typename Comp>
    struct InitPointer {
      using type = std::unique_ptr<CompInit<Comp>>;
    };
    Utils::ListToTuple<Utils::TransformList<CompList, InitPointer>> inits;
    
    template <typename Comp>
    auto &getInit() {
      return std::get<Utils::indexOf<CompList, Comp>>(inits);
    }
    
    template <typename Comp>
    auto &getInit() const {
      return std::get<Utils::indexOf<CompList, Comp>>(inits);
    }
  };
}

#endif
