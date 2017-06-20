//
//  event.hpp
//  Game Engine
//
//  Created by Indi Kernick on 11/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_event_event_hpp
#define engine_event_event_hpp

#include <cstdint>
#include <memory>
#include <cassert>
#include "../ID/type.hpp"

namespace Game {
  class Event {
  public:
    using Type = uint32_t;
    using Ptr = std::shared_ptr<Event>;
    
    Event() = default;
    virtual ~Event() = default;
    
    virtual Type getType() const = 0;
    virtual const char *getName() const = 0;
  };
  
  template <typename EventClass>
  using GetEventType = ID::TypeCounter<Event::Type, EventClass, Event>;
  
  template <typename EventClass, const char *NAME>
  class StaticEvent : public Event {
  public:
    using Ptr = std::shared_ptr<EventClass>;
    
    StaticEvent() = default;
    ~StaticEvent() {
      static_assert(std::is_base_of<StaticEvent, EventClass>::value);
    }
    
    Type getType() const override final {
      return GetEventType<EventClass>::get();
    }
    
    const char *getName() const override final {
      return NAME;
    }
  };
}

#endif
