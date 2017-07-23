//
//  terminal color.cpp
//  Game Engine
//
//  Created by Indi Kernick on 27/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "terminal color.hpp"

#include <cstdio>
#include <cassert>

#define ESC "\x1B["

void Utils::Term::cursorUp(const unsigned n) {
  std::printf(ESC "%uA", n);
}

void Utils::Term::cursorDown(const unsigned n) {
  std::printf(ESC "%uB", n);
}

void Utils::Term::cursorForward(const unsigned n) {
  std::printf(ESC "%uC", n);
}

void Utils::Term::cursorBack(const unsigned n) {
  std::printf(ESC "%uD", n);
}

void Utils::Term::cursorNextLine(const unsigned n) {
  std::printf(ESC "%uE", n);
}

void Utils::Term::cursorPrevLine(const unsigned n) {
  std::printf(ESC "%uF", n);
}

void Utils::Term::cursorHoriPos(const unsigned col) {
  std::printf(ESC "%uG", col);
}

void Utils::Term::cursorPos(const unsigned row, const unsigned col) {
  std::printf(ESC "%u;%uG", row, col);
}

void Utils::Term::cursorPos(const CursorPos pos) {
  std::printf(ESC "%u;%uG", pos.row, pos.col);
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

void Utils::Term::scrollUp(const unsigned n) {
  std::printf(ESC "%uS", n);
}

void Utils::Term::scrollDown(const unsigned n) {
  std::printf(ESC "%uT", n);
}

void Utils::Term::eraseDisplay(const Clear clear) {
  std::printf(ESC "%cJ", static_cast<uint8_t>(clear));
}

void Utils::Term::eraseLine(const Clear clear) {
  std::printf(ESC "%cK", static_cast<uint8_t>(clear));
}

void Utils::Term::videoReset() {
  std::printf(ESC "0m");
}

void Utils::Term::intensity(const Intensity intensity) {
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

void Utils::Term::italic(const bool on) {
  if (on) {
    std::printf(ESC "3m");
  } else {
    std::printf(ESC "23m");
  }
}

void Utils::Term::underline(const bool on) {
  if (on) {
    std::printf(ESC "4m");
  } else {
    std::printf(ESC "24m");
  }
}

void Utils::Term::blink(const Blink rate) {
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

void Utils::Term::videoNegative(const bool on) {
  if (on) {
    std::printf(ESC "7m");
  } else {
    std::printf(ESC "27m");
  }
}

void Utils::Term::conceal(const bool on) {
  if (on) {
    std::printf(ESC "8m");
  } else {
    std::printf(ESC "28m");
  }
}

void Utils::Term::strikethrough(const bool on) {
  if (on) {
    std::printf(ESC "9m");
  } else {
    std::printf(ESC "29m");
  }
}

void Utils::Term::primaryFont() {
  std::printf(ESC "10m");
}

void Utils::Term::alternativeFont(const unsigned n) {
  assert(n < 9);
  std::printf(ESC "%um", 11u + n);
}

void Utils::Term::textColor(const Color color) {
  std::printf(ESC "%um", 30u + static_cast<unsigned>(color));
}

void Utils::Term::textColor(const uint8_t r, const uint8_t g, const uint8_t b) {
  std::printf(ESC "38;2;%c;%c;%cm", r, g, b);
}

void Utils::Term::defaultTextColor() {
  std::printf(ESC "39m");
}

void Utils::Term::backColor(const Color color) {
  std::printf(ESC "%um", 40u + static_cast<unsigned>(color));
}

void Utils::Term::backColor(const uint8_t r, const uint8_t g, const uint8_t b) {
  std::printf(ESC "48;2;%c;%c;%cm", r, g, b);
}

void Utils::Term::defaultBackColor() {
  std::printf(ESC "49m");
}

#undef ESC
