//
//  terminal color.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 27/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include <cstdio>
#include <cassert>

#define ESC "\x1B["

inline void Utils::Term::cursorUp(const unsigned n) {
  std::printf(ESC "%uA", n);
}

inline void Utils::Term::cursorDown(const unsigned n) {
  std::printf(ESC "%uB", n);
}

inline void Utils::Term::cursorForward(const unsigned n) {
  std::printf(ESC "%uC", n);
}

inline void Utils::Term::cursorBack(const unsigned n) {
  std::printf(ESC "%uD", n);
}

inline void Utils::Term::cursorNextLine(const unsigned n) {
  std::printf(ESC "%uE", n);
}

inline void Utils::Term::cursorPrevLine(const unsigned n) {
  std::printf(ESC "%uF", n);
}

inline void Utils::Term::cursorHoriPos(const unsigned col) {
  std::printf(ESC "%uG", col);
}

inline void Utils::Term::cursorPos(const unsigned row, const unsigned col) {
  std::printf(ESC "%u;%uG", row, col);
}

inline void Utils::Term::cursorPos(const CursorPos pos) {
  std::printf(ESC "%u;%uG", pos.row, pos.col);
}

inline Utils::Term::CursorPos Utils::Term::getCursorPos() {
  std::fputs(ESC "6n", stdout);
  CursorPos pos;
  std::scanf(ESC "%u;%uR", &pos.row, &pos.col);
  return pos;
}

inline void Utils::Term::saveCursor() {
  std::fputs(ESC "s", stdout);
}

inline void Utils::Term::restoreCursor() {
  std::fputs(ESC "u", stdout);
}

inline void Utils::Term::hideCursor() {
  std::fputs(ESC "?25l", stdout);
}

inline void Utils::Term::showCursor() {
  std::fputs(ESC "?25h", stdout);
}

inline void Utils::Term::scrollUp(const unsigned n) {
  std::printf(ESC "%uS", n);
}

inline void Utils::Term::scrollDown(const unsigned n) {
  std::printf(ESC "%uT", n);
}

inline void Utils::Term::eraseDisplay(const Clear clear) {
  std::printf(ESC "%cJ", static_cast<uint8_t>(clear));
}

inline void Utils::Term::eraseLine(const Clear clear) {
  std::printf(ESC "%cK", static_cast<uint8_t>(clear));
}

inline void Utils::Term::videoReset() {
  std::fputs(ESC "0m", stdout);
}

inline void Utils::Term::intensity(const Intensity intensity) {
  switch (intensity) {
    case Intensity::NORMAL:
      std::fputs(ESC "22m", stdout);
      break;
    case Intensity::BOLD:
      std::fputs(ESC "1m", stdout);
      break;
    case Intensity::FAINT:
      std::fputs(ESC "2m", stdout);
  }
}

inline void Utils::Term::italic(const bool on) {
  if (on) {
    std::fputs(ESC "3m", stdout);
  } else {
    std::fputs(ESC "23m", stdout);
  }
}

inline void Utils::Term::underline(const bool on) {
  if (on) {
    std::fputs(ESC "4m", stdout);
  } else {
    std::fputs(ESC "24m", stdout);
  }
}

inline void Utils::Term::blink(const Blink rate) {
  switch (rate) {
    case Blink::OFF:
      std::fputs(ESC "25m", stdout);
      break;
    case Blink::SLOW:
      std::fputs(ESC "5m", stdout);
      break;
    case Blink::FAST:
      std::fputs(ESC "6m", stdout);
  }
}

inline void Utils::Term::videoNegative(const bool on) {
  if (on) {
    std::fputs(ESC "7m", stdout);
  } else {
    std::fputs(ESC "27m", stdout);
  }
}

inline void Utils::Term::conceal(const bool on) {
  if (on) {
    std::fputs(ESC "8m", stdout);
  } else {
    std::fputs(ESC "28m", stdout);
  }
}

inline void Utils::Term::strikethrough(const bool on) {
  if (on) {
    std::fputs(ESC "9m", stdout);
  } else {
    std::fputs(ESC "29m", stdout);
  }
}

inline void Utils::Term::primaryFont() {
  std::fputs(ESC "10m", stdout);
}

inline void Utils::Term::alternativeFont(const unsigned n) {
  assert(n < 9);
  std::printf(ESC "%um", 11u + n);
}

inline void Utils::Term::textColor(const Color color) {
  std::printf(ESC "%um", 30u + static_cast<unsigned>(color));
}

inline void Utils::Term::textColor(const uint8_t r, const uint8_t g, const uint8_t b) {
  std::printf(ESC "38;2;%c;%c;%cm", r, g, b);
}

inline void Utils::Term::defaultTextColor() {
  std::fputs(ESC "39m", stdout);
}

inline void Utils::Term::backColor(const Color color) {
  std::printf(ESC "%um", 40u + static_cast<unsigned>(color));
}

inline void Utils::Term::backColor(const uint8_t r, const uint8_t g, const uint8_t b) {
  std::printf(ESC "48;2;%c;%c;%cm", r, g, b);
}

inline void Utils::Term::defaultBackColor() {
  std::fputs(ESC "49m", stdout);
}

#undef ESC
