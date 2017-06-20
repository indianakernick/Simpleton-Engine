//
//  event.cpp
//  Game Engine
//
//  Created by Indi Kernick on 11/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "event.hpp"

#define GET_TYPE(class) \
Game::Event::Type Input::class::getType() const { \
  return Game::GetEventType<class>::get();\
}\

#define GET_NAME(class) \
const char *Input::class::getName() const { \
  return #class;\
}\

#define IMPL(class) \
GET_TYPE(class) \
GET_NAME(class)

IMPL(MouseDown)
IMPL(MouseUp)
IMPL(MouseMove)
IMPL(Scroll)
IMPL(KeyDown)
IMPL(KeyUp)
IMPL(Quit)

#undef IMPL
#undef GET_NAME
#undef GET_TYPE
