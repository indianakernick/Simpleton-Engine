//
//  message.cpp
//  Game Engine
//
//  Created by Indi Kernick on 4/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "message.hpp"

Game::Message::Message(const ID id, const Utils::Any &data)
  : data(data), id(id) {}

Game::Message::Message(const ID id, Utils::Any &&data)
  : data(std::move(data)), id(id) {}

Game::NullManager::NullManager()
  : std::runtime_error("Messenger::getManager returned a null pointer") {}

Game::MissingMessenger::MissingMessenger()
  : std::runtime_error("Tried to send a message to a Messenger that doesn't exist") {}
