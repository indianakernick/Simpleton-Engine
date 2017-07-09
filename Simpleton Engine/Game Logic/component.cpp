//
//  component.cpp
//  Game Engine
//
//  Created by Indi Kernick on 10/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "component.hpp"

#include "actor.hpp"

Game::MessageManager<Game::Component::ID> *Game::Component::getManager() const {
  return actor;
}
