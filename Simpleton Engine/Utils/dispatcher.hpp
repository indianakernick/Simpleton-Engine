//
//  dispatcher.hpp
//  Game Engine
//
//  Created by Indi Kernick on 16/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_dispatcher_hpp
#define engine_utils_dispatcher_hpp

#include <queue>
#include <tuple>
#include <vector>
#include <cassert>
#include <stdexcept>
#include <functional>
#include <experimental/tuple>
#include "member function.hpp"

/*
RetHandler is a type with an interface compatible with the following

struct RetHandler {
  RetHandler();
  
  void handleReturnValue(ListenerRet);
  ListenerRet getFinalReturnValue();
}
*/

namespace Utils {
  template <typename Listener, typename RetHandler, typename ListenerID = uint32_t>
  class Dispatcher;

  template <typename ListenerId, typename RetHandler, typename ListenerRet, typename ...ListenerArgs>
  class Dispatcher<ListenerRet(ListenerArgs...), RetHandler, ListenerId> {
  public:
    using Listener = std::function<ListenerRet (ListenerArgs...)>;
    using ListenerID = ListenerId;
    
    class BadListenerID final : public std::runtime_error {
    public:
      explicit BadListenerID(const ListenerID id)
        : std::runtime_error("Dispatcher::remListener was called with a bad ListenerID"), id(id) {}
      
      ListenerID getID() const {
        return id;
      }
      
    private:
      ListenerID id;
    };
    
    class BadListener final : public std::runtime_error {
    public:
      BadListener()
        : std::runtime_error("Dispatcher::addListener was called with a null listener") {}
    };
    
    class BadDispatchCall final : public std::logic_error {
    public:
      BadDispatchCall()
        : std::logic_error("Dispatcher::dispatch was called from an listener") {}
    };

    Dispatcher() = default;
    ~Dispatcher() = default;
    
    ///Add a listener
    ListenerID addListener(const Listener &listener) {
      if (listener == nullptr) {
        throw BadListener();
      }
      
      const ListenerID id = static_cast<ListenerID>(listeners.size());
      listeners.emplace_back(listener);
      return id;
    }
    
    ///Remove a listeners
    void remListener(const ListenerID id) {
      if (id >= listeners.size()) {
        throw BadListenerID(id);
      }
      
      if (dispatching) {
        oldListeners.push_back(id);
      } else {
        listeners[id] = nullListener;
      }
    }
    
    ///Send a message to the listeners
    ListenerRet dispatch(const ListenerArgs... args) {
      if (dispatching) {
        throw BadDispatchCall();
      }
      
      dispatching = true;
      
      if constexpr (std::is_void<ListenerRet>::value) {
        const auto end = listeners.cend();
        for (auto l = listeners.cbegin(); l != end; ++l) {
          (*l)(args...);
        }
        dispatching = false;
        remOldListeners();
      } else {
        RetHandler retHandler;
        const auto end = listeners.cend();
        for (auto l = listeners.cbegin(); l != end; ++l) {
          retHandler.handleReturnValue((*l)(args...));
        }
        dispatching = false;
        remOldListeners();
        
        return retHandler.getFinalReturnValue();
      }
    }
    
  private:
    std::vector<Listener> listeners;
    //listeners that will be removed after dispatch finishes
    std::vector<ListenerID> oldListeners;
    //dispatch is currently running
    bool dispatching = false;
    
    void remOldListeners() {
      for (auto l = oldListeners.cbegin(); l != oldListeners.cend(); ++l) {
        listeners[*l] = nullListener;
      }
      oldListeners.clear();
    }
    
    static ListenerRet nullListener(ListenerArgs...) {
      if constexpr (!std::is_void<ListenerRet>::value) {
        return {};
      }
    }
  };

  ///An action that can be observed
  template <typename ListenerID, typename ...Args>
  using Observable = Dispatcher<void (Args...), void, ListenerID>;

  class ConfirmableRetHandler {
  public:
    ConfirmableRetHandler() = default;
    ~ConfirmableRetHandler() = default;
    
    void handleReturnValue(const bool val) {
      out = out && val;
    }
    
    bool getFinalReturnValue() const {
      return out;
    }

  private:
    bool out = true;
  };

  ///An action that can be confirmed to happen
  template <typename ListenerID, typename ...Args>
  using Confirmable = Dispatcher<bool (Args...), ConfirmableRetHandler, ListenerID>;

  ///Divides listeners into groups. A message can be dispatched to each group individually.
  ///Many Dispatchers will perform better than one GroupDispatcher
  template <typename Listener, typename RetHandler, typename GroupID = uint32_t, typename ListenerID = uint32_t>
  class GroupDispatcher;

  template <typename GroupId, typename ListenerId, typename RetHandler, typename ListenerRet, typename ...ListenerArgs>
  class GroupDispatcher<ListenerRet(ListenerArgs...), RetHandler, GroupId, ListenerId> {
  public:
    static_assert(std::is_integral<ListenerId>::value);
    static_assert(std::is_integral<GroupId>::value);

    using Listener = std::function<ListenerRet (ListenerArgs...)>;
    using ListenerID = ListenerId;
    using GroupID = GroupId;
    
    //used for calculating IDs
    static constexpr ListenerID MAX_LISTENERS_PER_GROUP = 1 << (sizeof(ListenerID) * 4);
    static constexpr GroupID MAX_GROUPS = std::min(
      MAX_LISTENERS_PER_GROUP,
      std::numeric_limits<GroupID>::max()
    );
    
    class BadListenerID final : public std::runtime_error {
    public:
      explicit BadListenerID(const ListenerID id)
        : std::runtime_error("GroupDispatcher::remListener was called with a bad ListenerID"), id(id) {}
      
      ListenerID getID() const {
        return id;
      }
      
    private:
      ListenerID id;
    };
    
    class BadGroupID final : public std::runtime_error {
    public:
      explicit BadGroupID(const GroupID id)
        : std::runtime_error("A group with this GroupID doesn't exist"), id(id) {}
      
      GroupID getID() const {
        return id;
      }
      
    private:
      GroupID id;
    };
    
    class BadListener final : public std::runtime_error {
    public:
      BadListener()
        : std::runtime_error("GroupDispatcher::addListener was called with a null listener") {}
    };
    
    class BadDispatchCall final : public std::logic_error {
    public:
      BadDispatchCall()
        : std::logic_error("GroupDispatcher::dispatch was called from an listener") {}
    };

    GroupDispatcher() = default;
    ~GroupDispatcher() = default;
    
    ///Create a group. This will also create all previous groups if they don't exist yet
    void createGroup(const GroupID groupID) {
      const size_t group = static_cast<size_t>(groupID);
      while (groups.size() <= group) {
        groups.emplace_back();
      }
    }
    
    ///Add a listener to a group. Create the group if it doesn't exist
    ListenerID addListenerAndCreateGroup(const GroupID groupID, const Listener &listener) {
      createGroup(groupID);
      return addListener(groupID, listener);
    }
    
    ///Add a listener to a group. Throw if that group doesn't exist
    ListenerID addListener(const GroupID groupID, const Listener &listener) {
      if (listener == nullptr) {
        throw BadListener();
      }
      const size_t group = static_cast<size_t>(groupID);
      if (group >= groups.size()) {
        throw BadGroupID(groupID);
      }
      const ListenerID listenerID = static_cast<ListenerID>(
        group * MAX_LISTENERS_PER_GROUP + groups[group].size()
      );
      groups[group].emplace_back(listener);
      return listenerID;
    }
    
    ///Remove a listener
    void remListener(const ListenerID listenerID) {
      const size_t group = listenerID / MAX_LISTENERS_PER_GROUP;
      const size_t listener = listenerID % MAX_LISTENERS_PER_GROUP;
      
      if (group >= groups.size() || listener >= groups[group].size()) {
        throw BadListenerID(listenerID);
      }
      
      if (dispatching) {
        oldListeners.push_back(listenerID);
      } else {
        groups[group][listener] = nullListener;
      }
    }

    ///Send a message to a group of listeners
    ListenerRet dispatch(const GroupID groupID, const ListenerArgs... args) {
      if (dispatching) {
        dispatchArgs.emplace(groupID, args...);
        return;
      }
      
      dispatching = true;
      
      const size_t group = static_cast<GroupID>(groupID);
      if (group >= groups.size()) {
        dispatching = false;
        if constexpr (std::is_void<ListenerRet>::value) {
          return;
        } else {
          const RetHandler retHandler;
          return retHandler.getFinalReturnValue();
        }
      }
      const Listeners &listeners = groups[group];
      
      if constexpr (std::is_void<ListenerRet>::value) {
        const auto end = listeners.cend();
        for (auto l = listeners.cbegin(); l != end; ++l) {
          (*l)(args...);
        }
        dispatching = false;
        remOldListeners();
        callDispatch();
      } else {
        RetHandler retHandler;
        const auto end = listeners.cend();
        for (auto l = listeners.cbegin(); l != end; ++l) {
          retHandler.handleReturnValue((*l)(args...));
        }
        dispatching = false;
        remOldListeners();
        callDispatch();
        
        return retHandler.getFinalReturnValue();
      }
    }
    
  private:
    using Listeners = std::vector<Listener>;
    using Groups = std::vector<Listeners>;
    Groups groups;
    //listeners that will be removed after dispatch finishes
    std::vector<ListenerID> oldListeners;
    
    //when dispatch is called from a listener, the arguments are pushed onto
    //this queue. Dispatch will then be called for each tuple
    using ArgsTuple = std::tuple<GroupID, ListenerArgs...>;
    std::queue<ArgsTuple> dispatchArgs;
    
    //dispatch is currently running
    bool dispatching = false;
    
    void remOldListeners() {
      for (auto l = oldListeners.cbegin(); l != oldListeners.cend(); ++l) {
        const size_t listenerID = *l;
        const size_t group = listenerID / MAX_LISTENERS_PER_GROUP;
        const size_t listener = listenerID % MAX_LISTENERS_PER_GROUP;
        groups[group][listener] = nullListener;
      }
      oldListeners.clear();
    }
    
    void callDispatch() {
      while (!dispatchArgs.empty()) {
        const ArgsTuple args = dispatchArgs.front();
        dispatchArgs.pop();
        std::experimental::apply(memFun(this, &GroupDispatcher::dispatch), args);
      }
    }
    
    static ListenerRet nullListener(ListenerArgs...) {
      if constexpr (!std::is_void<ListenerRet>::value) {
        const RetHandler retHandler;
        return retHandler.getFinalReturnValue();
      }
    }
  };
}

#endif
