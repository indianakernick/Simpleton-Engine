//
//  component.hpp
//  Game Engine
//
//  Created by Indi Kernick on 10/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_game_logic_component_hpp
#define engine_game_logic_component_hpp

#include <memory>
#include "message.hpp"
#include "../ID/type.hpp"

namespace Game {
  class Actor;

  class Component : public Messenger<uint8_t> {
  friend Actor;
  public:
    using Ptr = std::shared_ptr<Component>;
    using ID = uint8_t;

    //Component inherits Messenger<Component::ID>
    static_assert(std::is_same<ID, Messenger::ID>::value);

    Component() = default;
    virtual ~Component() = default;
    
    virtual void init() = 0;
    virtual void quit() = 0;
    virtual void update(uint64_t) = 0;
    
  protected:
    Actor *actor = nullptr;
    
    template <typename ComponentClass, typename MessageClass>
    void sendMessage();
    template <typename ComponentClass, typename MessageClass, typename MessageData>
    void sendMessage(MessageData &&);
    
  private:
    using Messenger::onMessage;
    using Messenger::sendMessage;
  
    MessageManager<ID> *getManager() const override;
  };
  
  template <typename ComponentClass>
  using GetComponentID = ID::TypeCounter<Component::ID, ComponentClass, Component>;
  
  template <typename ComponentClass, typename MessageClass>
  void Component::sendMessage() {
    sendMessage(
      GetComponentID<ComponentClass>::get(),
      Game::Message(
        GetMessageID<MessageClass>::get()
      )
    );
  }
  
  template <typename ComponentClass, typename MessageClass, typename MessageData>
  void Component::sendMessage(MessageData &&data) {
    sendMessage(
      GetComponentID<ComponentClass>::get(),
      Game::Message(
        GetMessageID<MessageClass>::get(),
        std::experimental::any(std::forward<MessageData>(data))
      )
    );
  }
}

#endif
