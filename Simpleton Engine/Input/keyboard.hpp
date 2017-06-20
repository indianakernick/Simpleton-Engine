//
//  keyboard.hpp
//  Game Engine
//
//  Created by Indi Kernick on 4/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_input_keyboard_hpp
#define engine_input_keyboard_hpp

#include "../Utils/bool enable.hpp"

namespace Input {
  //the same standard that javascript uses (whatever that is)
  namespace Key {
    using Type = uint8_t;
    enum : Type {
      UNKNOWN = 0,
      
      BACKSPACE = 8,
      TAB = 9,
      ENTER = 13,
      SHIFT = 16,
      CONTROL = 17,
      ALT = 18,
      CAPS_LOCK = 20,
      ESCAPE = 27,
      SPACE = 32,
    
      LEFT = 37,
      UP,
      RIGHT,
      DOWN,
    
      NUM_0 = 48,
      NUM_1,
      NUM_2,
      NUM_3,
      NUM_4,
      NUM_5,
      NUM_6,
      NUM_7,
      NUM_8,
      NUM_9,
      
      A = 65,
      B,
      C,
      D,
      E,
      F,
      G,
      H,
      I,
      J,
      K,
      L,
      M,
      N,
      O,
      P,
      Q,
      R,
      S,
      T,
      U,
      V,
      W,
      X,
      Y,
      Z,
      
      LEFT_META = 91,
      RIGHT_META = 93,
      
      F1 = 112,
      F2,
      F3,
      F4,
      F5,
      F6,
      F7,
      F8,
      F9,
      F10,
      F11,
      F12,
      
      SEMICOLON = 186,
      EQUAL,
      COMMA,
      MINUS,
      DOT,
      PERIOD = DOT,
      SLASH,
      BACK_QUOTE,
      
      LEFT_BRACKET = 219,
      BACK_SLASH,
      RIGHT_BRACKET,
      QUOTE,
      
      NUM_OF_KEYS
    };
  }
  
  namespace Mod {
    using Type = uint8_t;
    enum : Type {
      SHIFT      = 0b1,
      CONTROL    = 0b10,
      ALT        = 0b100,
      CAPS_LOCK  = 0b1000,
      LEFT_META  = 0b10000,
      RIGHT_META = 0b100000
    };
  }
  
  const char NOT_PRINTABLE = '\0';
  
  char codeToChar(Key::Type key, Mod::Type modifiers);
  const char *codeToWord(Key::Type key);
  Mod::Type getModifiers(const bool *keyStates);
  
  bool isArrow(Key::Type);
  bool isAlpha(Key::Type);
  bool isNum(Key::Type);
  bool isFunc(Key::Type);
}

#endif
