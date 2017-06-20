//
//  terminal color.cpp
//  Game Engine
//
//  Created by Indi Kernick on 27/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "terminal color.hpp"

#define ESC "\x1B["

void Utils::Term::cursorUp(unsigned n) {
  std::printf(ESC "%uA", n);
}

void Utils::Term::cursorDown(unsigned n) {
  std::printf(ESC "%uB", n);
}

void Utils::Term::cursorForward(unsigned n) {
  std::printf(ESC "%uC", n);
}

void Utils::Term::cursorBack(unsigned n) {
  std::printf(ESC "%uD", n);
}

void Utils::Term::cursorNextLine(unsigned n) {
  std::printf(ESC "%uE", n);
}

void Utils::Term::cursorPrevLine(unsigned n) {
  std::printf(ESC "%uF", n);
}

void Utils::Term::cursorHoriPos(unsigned col) {
  std::printf(ESC "%uG", col);
}

void Utils::Term::cursorPos(unsigned row, unsigned col) {
  std::printf(ESC "%u;%uG", row, col);
}

Utils::Term::CursorPos Utils::Term::getCursorPos() {
  std::printf(ESC "6n");
  CursorPos pos;
  std::scanf(ESC "%u;%uR", &pos.row, &pos.col);
  return pos;
}

void Utils::Term::saveCursor() {
  std::printf(ESC "s");
}

void Utils::Term::restoreCursor() {
  std::printf(ESC "u");
}

void Utils::Term::hideCursor() {
  std::printf(ESC "?25l");
}

void Utils::Term::showCursor() {
  std::printf(ESC "?25h");
}

void Utils::Term::scrollUp(unsigned n) {
  std::printf(ESC "%uS", n);
}

void Utils::Term::scrollDown(unsigned n) {
  std::printf(ESC "%uT", n);
}

void Utils::Term::eraseDisplay(Clear clear) {
  std::printf(ESC "%cJ", static_cast<uint8_t>(clear));
}

void Utils::Term::eraseLine(Clear clear) {
  std::printf(ESC "%cK", static_cast<uint8_t>(clear));
}

void Utils::Term::videoReset() {
  std::printf(ESC "0m");
}

void Utils::Term::intensity(Intensity intensity) {
  switch (intensity) {
    case Intensity::NORMAL:
      std::printf(ESC "22m");
      break;
    case Intensity::BOLD:
      std::printf(ESC "1m");
      break;
    case Intensity::FAINT:
      std::printf(ESC "2m");
  }
}

void Utils::Term::italic(bool on) {
  if (on) {
    std::printf(ESC "3m");
  } else {
    std::printf(ESC "23m");
  }
}

void Utils::Term::underline(bool on) {
  if (on) {
    std::printf(ESC "4m");
  } else {
    std::printf(ESC "24m");
  }
}

void Utils::Term::blink(Blink rate) {
  switch (rate) {
    case Blink::OFF:
      std::printf(ESC "25m");
      break;
    case Blink::SLOW:
      std::printf(ESC "5m");
      break;
    case Blink::FAST:
      std::printf(ESC "6m");
  }
}

void Utils::Term::videoNegative(bool on) {
  if (on) {
    std::printf(ESC "7m");
  } else {
    std::printf(ESC "27m");
  }
}

void Utils::Term::conceal(bool on) {
  if (on) {
    std::printf(ESC "8m");
  } else {
    std::printf(ESC "28m");
  }
}

void Utils::Term::strikethrough(bool on) {
  if (on) {
    std::printf(ESC "9m");
  } else {
    std::printf(ESC "29m");
  }
}

void Utils::Term::primaryFont() {
  std::printf(ESC "10m");
}

void Utils::Term::alternativeFont(unsigned n) {
  assert(n < 9);
  std::printf(ESC "%um", 11 + n);
}

void Utils::Term::textColor(Color color) {
  std::printf(ESC "%um", 30u + static_cast<unsigned>(color));
}

void Utils::Term::textColor(uint8_t r, uint8_t g, uint8_t b) {
  std::printf(ESC "38;2;%c;%c;%cm", r, g, b);
}

void Utils::Term::defaultTextColor() {
  std::printf(ESC "39m");
}

void Utils::Term::backColor(Color color) {
  std::printf(ESC "%um", 40u + static_cast<unsigned>(color));
}

void Utils::Term::backColor(uint8_t r, uint8_t g, uint8_t b) {
  std::printf(ESC "48;2;%c;%c;%cm", r, g, b);
}

void Utils::Term::defaultBackColor() {
  std::printf(ESC "49m");
}

#undef ESC
