//
//  keyboard.cpp
//  Game Engine
//
//  Created by Indi Kernick on 4/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "keyboard.hpp"

char Input::codeToChar(Key::Type key, Mod::Type modifiers) {
  static const char SHIFT_NUM_TABLE[] = {
    '!','@','#','$','%','^','&','*','(',')'
  };
  static const char SHIFT_SEMICOLON_TABLE[] = {
    ':','+','<','_','>','?','~'
  };
  static const char SEMICOLON_TABLE[] = {
    ';','=',',','-','.','/','`'
  };
  static const char SHIFT_LEFT_BRACKET_TABLE[] = {
    '{','|','}','"'
  };
  static const char LEFT_BRACKET_TABLE[] = {
    '[','\\',']','\''
  };
  
  bool capsLock = modifiers & Mod::CAPS_LOCK;
  bool shift = modifiers & Mod::SHIFT;
  
  using namespace Key;
  
  if (isAlpha(key)) {
    if (capsLock || shift) {
      return key;
    } else {
      return key + 32;
    }
  } else if (isNum(key)) {
    if (shift) {
      return SHIFT_NUM_TABLE[key - NUM_0];
    } else {
      return key;
    }
  } else if (key >= SEMICOLON && key <= BACK_QUOTE) {
    if (shift) {
      return SHIFT_SEMICOLON_TABLE[key - SEMICOLON];
    } else {
      return SEMICOLON_TABLE[key - SEMICOLON];
    }
  } else if (key >= LEFT_BRACKET && key <= QUOTE) {
    if (shift) {
      return SHIFT_LEFT_BRACKET_TABLE[key - LEFT_BRACKET];
    } else {
      return LEFT_BRACKET_TABLE[key - LEFT_BRACKET];
    }
  }
  
  return NOT_PRINTABLE;
}

Input::Mod::Type Input::getModifiers(const bool *keyStates) {
  return Utils::boolEnable(keyStates[Key::SHIFT],      Mod::SHIFT     ) |
         Utils::boolEnable(keyStates[Key::CONTROL],    Mod::CONTROL   ) |
         Utils::boolEnable(keyStates[Key::ALT],        Mod::ALT       ) |
         Utils::boolEnable(keyStates[Key::CAPS_LOCK],  Mod::CAPS_LOCK ) |
         Utils::boolEnable(keyStates[Key::LEFT_META],  Mod::LEFT_META ) |
         Utils::boolEnable(keyStates[Key::RIGHT_META], Mod::RIGHT_META);
}

const char *Input::codeToWord(Key::Type key) {
  if (isArrow(key)) {
    static const char *ARROW[] = {
      "LEFT","UP","RIGHT","DOWN"
    };
    return ARROW[key - Key::LEFT];
  } else if (isNum(key)) {
    static const char *NUM[] = {
      "0","1","2","3","4","5","6","7","8","9"
    };
    return NUM[key - Key::NUM_0];
  } else if (isAlpha(key)) {
    static const char *ALPHA[] = {
      "A","B","C","D","E","F","G","H","I","J","K","L","M",
      "N","O","P","Q","R","S","T","U","V","W","X","Y","Z"
    };
    return ALPHA[key - Key::A];
  } else if (isFunc(key)) {
    static const char *FUNCTION[] = {
      "F1","F2","F3","F4", "F5", "F6",
      "F7","F8","F9","F10","F11","F12",
    };
    return FUNCTION[key - Key::F1];
  } else if (key >= Key::SEMICOLON && key <= Key::BACK_QUOTE) {
    static const char *SEMICOLON[] = {
      ":","=",",","-",".","/","`"
    };
    return SEMICOLON[key - Key::SEMICOLON];
  } else if (key >= Key::LEFT_BRACKET && key <= Key::QUOTE) {
    static const char *LEFT_BRACKET[] = {
      "[","\\","]","'"
    };
    return LEFT_BRACKET[key - Key::LEFT_BRACKET];
  } else {
    using namespace Key;
    #define CASE(key) \
      case key: \
        return #key
    switch (key) {
      CASE(BACKSPACE);
      CASE(TAB);
      CASE(ENTER);
      CASE(SHIFT);
      CASE(CONTROL);
      CASE(ALT);
      CASE(CAPS_LOCK);
      CASE(ESCAPE);
      CASE(SPACE);
      CASE(LEFT_META);
      CASE(RIGHT_META);
    }
    #undef CASE
  }
  
  return "";
}

bool Input::isArrow(Key::Type key) {
  return key >= Key::LEFT && key <= Key::DOWN;
}

bool Input::isAlpha(Key::Type key) {
  return key >= Key::A && key <= Key::Z;
}

bool Input::isNum(Key::Type key) {
  return key >= Key::NUM_0 && key <= Key::NUM_9;
}

bool Input::isFunc(Key::Type key) {
  return key >= Key::F1 && key <= Key::F12;
}
