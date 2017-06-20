//
//  manager.cpp
//  Game Engine
//
//  Created by Indi Kernick on 11/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "manager.hpp"

using namespace Game;

const Event::Type EventManager::ANY_TYPE = GetEventType<EventManager>::get();
std::unique_ptr<EventManager> evtMan = nullptr;

EventManager::EventManager(const uint64_t timeLimit)
  : timeLimit(timeLimit) {
  dispatcher.createGroup(ANY_TYPE);
}

void EventManager::update() {
  PROFILE(Game::EventManager::update);
  
  Time::StopWatch<std::chrono::nanoseconds> stopwatch(true);
  const uint8_t processingQueue = currentQueue;
  currentQueue = (currentQueue + 1) % 2;
  
  while (!queue[processingQueue].empty()) {
    const Event::Ptr event = queue[processingQueue].front();
    queue[processingQueue].pop_front();
    emitNow(event);
    if (stopwatch.get() >= timeLimit) {
      break;
    }
  }
  
  if (!queue[processingQueue].empty()) {
    queue[currentQueue].insert(
      queue[currentQueue].begin(),
      queue[processingQueue].cbegin(),
      queue[processingQueue].cend()
    );
    queue[processingQueue].clear();
  }
}

void EventManager::emit(const Event::Ptr msg) {
  assert(msg);
  PROFILE(Game::EventManager::emit);
  
  queue[currentQueue].push_back(msg);
}

void EventManager::emitNow(const Event::Ptr msg) {
  assert(msg);
  PROFILE(Game::EventManager::emitNow);
  
  dispatcher.dispatch(msg->getType(), msg);
  dispatcher.dispatch(ANY_TYPE, msg);
}

EventManager::ListenerID EventManager::addListener(
  const Event::Type type,
  const Listener &listener
) {
  PROFILE(Game::EventManager::addListener);
  return dispatcher.addListenerAndCreateGroup(type, listener);
}

void EventManager::remListener(const ListenerID id) {
  PROFILE(Game::EventManager::remListener);
  dispatcher.remListener(id);
}

EventManager::ListenerID EventManager::addAnyListener(const Listener &listener) {
  PROFILE(Game::EventManager::addAnyListener);
  return dispatcher.addListenerAndCreateGroup(ANY_TYPE, listener);
}
