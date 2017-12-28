//
//  events.cpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 28/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "events.hpp"

#include <SDL2/SDL_events.h>

bool SDL::keyDown(const SDL_Event &e, const SDL_Scancode scancode) {
  return e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.scancode == scancode;
}

bool SDL::keyUp(const SDL_Event &e, const SDL_Scancode scancode) {
  return e.type == SDL_KEYUP && e.key.keysym.scancode == scancode;
}

bool SDL::keyDown(const SDL_Event &e) {
  return e.type == SDL_KEYDOWN && e.key.repeat == 0;
}

bool SDL::keyUp(const SDL_Event &e) {
  return e.type == SDL_KEYUP;
}

SDL_Scancode SDL::keyCode(const SDL_KeyboardEvent &e) {
  return e.keysym.scancode;
}
