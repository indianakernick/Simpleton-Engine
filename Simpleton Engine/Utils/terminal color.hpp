//
//  terminal color.hpp
//  Game Engine
//
//  Created by Indi Kernick on 27/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_terminal_color_hpp
#define engine_utils_terminal_color_hpp

#include <cstdio>
#include <cstdint>
#include <cassert>

//ANSI escape codes!

namespace Utils::Term {
  void cursorUp(unsigned = 1);
  void cursorDown(unsigned = 1);
  void cursorForward(unsigned = 1);
  void cursorBack(unsigned = 1);
  void cursorNextLine(unsigned = 1);
  void cursorPrevLine(unsigned = 1);
  void cursorHoriPos(unsigned = 1);
  void cursorPos(unsigned = 1, unsigned = 1);
  
  struct CursorPos {
    unsigned row;
    unsigned col;
  };
  
  CursorPos getCursorPos();
  
  void saveCursor();
  void restoreCursor();
  
  void hideCursor();
  void showCursor();
  
  void scrollUp(unsigned = 1);
  void scrollDown(unsigned = 1);
  
  enum class Clear : uint8_t {
    TO_END,
    TO_BEG,
    ALL
  };
  
  void eraseDisplay(Clear = Clear::TO_END);
  void eraseLine(Clear = Clear::TO_END);
  
  void videoReset();
    
  enum class Intensity {
    NORMAL,
    BOLD,
    FAINT
  };
  
  void intensity(Intensity);
  void italic(bool);
  void underline(bool);
  
  enum class Blink {
    OFF,
    SLOW,
    FAST
  };
  
  void blink(Blink);
  void videoNegative(bool);
  void conceal(bool);
  void strikethrough(bool);
  void primaryFont();
  void alternativeFont(unsigned);
  
  enum class Color : uint8_t {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
  };
  
  void textColor(Color);
  void textColor(uint8_t, uint8_t, uint8_t);
  void defaultTextColor();
  void backColor(Color);
  void backColor(uint8_t, uint8_t, uint8_t);
  void defaultBackColor();
}

#endif
