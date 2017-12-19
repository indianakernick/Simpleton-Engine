//
//  message.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 4/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "message.hpp"

Game::Message::Message(const ID id, const std::experimental::any &data)
  : data(data), id(id) {}

Game::Message::Message(const ID id, std::experimental::any &&data)
  : data(std::move(data)), id(id) {}

Game::NullManager::NullManager()
  : std::runtime_error("Messenger::getManager returned a null pointer") {}

Game::MissingMessenger::MissingMessenger()
  : std::runtime_error("Tried to send a message to a Messenger that doesn't exist") {}
