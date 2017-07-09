//
//  actor.hpp
//  Game Engine
//
//  Created by Indi Kernick on 20/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_game_logic_actor_hpp
#define engine_game_logic_actor_hpp

#include <queue>
#include <memory>
#include <vector>
#include "component.hpp"
#include "../Utils/safe down cast.hpp"

namespace Game {
  class MissingComponent final : public std::runtime_error {
  public:
    explicit MissingComponent(const char *);
  };
  
  class DuplicateComponent final : public std::runtime_error {
  public:
    DuplicateComponent();
  };
  
  class BadActorPtr final : public std::runtime_error {
  public:
    explicit BadActorPtr(const char *);
  };

  using Components = std::vector<Component::Ptr>;

  class Actor final : public MessageManager<Component::ID> {
  public:
    using Ptr = std::shared_ptr<Actor>;
    using ID = uint64_t;
    
    static const ID NULL_ID;
    
    explicit Actor(ID);
    Actor(ID, const Components &);
    Actor(ID, Components &&);
    ~Actor() = default;
    
    ///Get a shared_ptr to a component
    template <typename Comp>
    std::enable_if_t<
      std::is_base_of<Component, Comp>::value &&
      !std::is_same<Component, Comp>::value,
      std::shared_ptr<Comp>
    >
    getComponentShared() const {
      const Component::ID compID = GetComponentID<Comp>::get();
      if (compID < components.size() && components[compID]) {
        return Utils::safeDownCast<Comp>(components[compID]);
      }
      throw MissingComponent("Tried to get a component that doesn't exist");
    }
    
    ///Get a weak_ptr to a component
    template <typename Comp>
    std::weak_ptr<Comp> getComponentWeak() const {
      return getComponentShared<Comp>();
    }
    
    ///Get a pointer to a component
    template <typename Comp>
    Comp *getComponentPtr() const {
      const Component::ID compID = GetComponentID<Comp>::get();
      if (compID < components.size() && components[compID]) {
        return Utils::safeDownCast<Comp>(components[compID].get());
      }
      throw MissingComponent("Tried to get a component that doesn't exist");
    }
    
    ///Get a reference to a component
    template <typename Comp>
    Comp &getComponentRef() const {
      return *getComponentPtr<Comp>();
    }
    
    ///Add a new component
    template <typename Comp>
    std::enable_if_t<
      std::is_base_of<Component, Comp>::value &&
      !std::is_same<Component, Comp>::value
    >
    addComponent(const std::shared_ptr<Comp> comp) {
      assert(comp);
      if (comp->actor == this) {
        throw BadActorPtr("Cannot add component to same actor twice");
      } else if (comp->actor != nullptr) {
        throw BadActorPtr("Cannot add component to more than one actor");
      }
      
      const size_t compID = GetComponentID<Comp>::get();
      if (compID < components.size()) {
        if (components[compID]) {
          throw DuplicateComponent();
        } else {
          components[compID] = comp;
        }
      } else {
        do {
          components.emplace_back();
        } while (components.size() <= compID);
        components[compID] = comp;
      }
      
      comp->actor = this;
    }
    
    ///Remove a component and return shared_ptr to it
    template <typename Comp>
    std::enable_if_t<
      std::is_base_of<Component, Comp>::value &&
      !std::is_same<Component, Comp>::value,
      std::shared_ptr<Comp>
    >
    remComponent() {
      const size_t compID = GetComponentID<Comp>::get();
      if (compID < components.size() && components[compID]) {
        const std::shared_ptr<Comp> out = components[compID];
        components[compID] = nullptr;
        return out;
      } else {
        return nullptr;
      }
    }
    
    ID getID() const;
    void flushMessages();
    void initComponents();
    void quitComponents();
    void updateComponents(uint64_t);
    
  private:
    ID id;
    Components components;
    
    void setActorPtr();
    
    using MessageManager::sendMessage;
    using MessageManager::flushMessages;
    
    Messenger<Component::ID> *getMessenger(Component::ID) const override;
  };
}

#endif
