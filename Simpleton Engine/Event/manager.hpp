//
//  manager.hpp
//  Game Engine
//
//  Created by Indi Kernick on 11/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_event_manager_hpp
#define engine_event_manager_hpp

#include <deque>
#include <iosfwd>
#include "event.hpp"
#include "../Utils/profiler.hpp"
#include "../Utils/dispatcher.hpp"
#include "../Utils/safe down cast.hpp"
#include "../Utils/function traits.hpp"
#include "../Time/stopwatch.hpp"

namespace Game {
  class EventManager {
  private:
    using DispatcherImpl = Utils::GroupDispatcher<void (const Event::Ptr), void, Event::Type, uint32_t>;
    
  public:
    using Listener = DispatcherImpl::Listener;
    using ListenerID = DispatcherImpl::ListenerID;

    explicit EventManager(Time::Rep = std::numeric_limits<Time::Rep>::max());
    ~EventManager() = default;
    
    ///Write the names of all events in the queue to the given stream
    void printQueue(std::ostream &, char = '\n') const;
    
    ///Call the event listeners for each event. Never takes longer than
    ///timeLimit nanoseconds to process.
    void update();
    
    ///Emit an event
    void emit(Event::Ptr);
    ///Emit an event immediately
    void emitNow(Event::Ptr);
    
    ///Add a event listener
    ListenerID addListener(Event::Type, const Listener &);
    ///Remove a event listener
    void remListener(ListenerID);
    
    ///Add a universal event listener. A listener for any event
    ListenerID addAnyListener(const Listener &);
    
    ///Emit an event
    template <typename EventClass, typename ...Args>
    void emit(Args &&... args) {
      PROFILE(Game::EventManager::emit);
    
      emit(std::make_shared<EventClass>(std::forward<Args>(args)...));
    }
    
    ///Emit an event immediately
    template <typename EventClass, typename ...Args>
    void emitNow(Args &&... args) {
      PROFILE(Game::EventManager::emitNow);
      
      const std::shared_ptr<EventClass> event = std::make_shared<EventClass>(std::forward<Args>(args)...);
      
      dispatcher.dispatch(GetEventType<EventClass>::get(), event);
      dispatcher.dispatch(ANY_TYPE, event);
    }
    
    ///Emit an event immediately
    template <typename EventClass>
    void emitNow(const std::shared_ptr<EventClass> event) {
      assert(event);
      PROFILE(Game::EventManager::emitNow);
      
      dispatcher.dispatch(GetEventType<EventClass>::get(), event);
      dispatcher.dispatch(ANY_TYPE, event);
    }
    
    ///Add a event listener
    template <typename Function>
    ListenerID addListener(Function &&listener) {
      PROFILE(Game::EventManager::addListener);
    
      using EventClass = typename Utils::function_arg<Function, 0>::element_type;
      return addListener(
        GetEventType<EventClass>::get(),
        [listener = std::forward<Function>(listener)] (const Event::Ptr event) {
          listener(Utils::safeDownCast<EventClass>(event));
        }
      );
    }
    
  private:
    //the type of an event listener listening to any event
    static const Event::Type ANY_TYPE;
  
    DispatcherImpl dispatcher;
    std::deque<Event::Ptr> queue[2];
    Time::Rep timeLimit;
    size_t currentQueue = 0;
  };
  
  extern std::unique_ptr<Game::EventManager> evtMan;
}

#endif
