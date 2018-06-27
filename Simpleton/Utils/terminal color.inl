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
  std::fprintf(TERM_STREAM, ESC "%uA", n);
}

inline void Utils::Term::cursorDown(const unsigned n) {
  std::fprintf(TERM_STREAM, ESC "%uB", n);
}

inline void Utils::Term::cursorForward(const unsigned n) {
  std::fprintf(TERM_STREAM, ESC "%uC", n);
}

inline void Utils::Term::cursorBack(const unsigned n) {
  std::fprintf(TERM_STREAM, ESC "%uD", n);
}

inline void Utils::Term::cursorNextLine(const unsigned n) {
  std::fprintf(TERM_STREAM, ESC "%uE", n);
}

inline void Utils::Term::cursorPrevLine(const unsigned n) {
  std::fprintf(TERM_STREAM, ESC "%uF", n);
}

inline void Utils::Term::cursorHoriPos(const unsigned col) {
  std::fprintf(TERM_STREAM, ESC "%uG", col);
}

inline void Utils::Term::cursorPos(const unsigned row, const unsigned col) {
  std::fprintf(TERM_STREAM, ESC "%u;%uG", row, col);
}

inline void Utils::Term::cursorPos(const CursorPos pos) {
  std::fprintf(TERM_STREAM, ESC "%u;%uG", pos.row, pos.col);
}

inline Utils::Term::CursorPos Utils::Term::getCursorPos() {
  std::fputs(ESC "6n", TERM_STREAM);
  CursorPos pos;
  std::scanf(ESC "%u;%uR", &pos.row, &pos.col);
  return pos;
}

inline void Utils::Term::saveCursor() {
  std::fputs(ESC "s", TERM_STREAM);
}

inline void Utils::Term::restoreCursor() {
  std::fputs(ESC "u", TERM_STREAM);
}

inline void Utils::Term::hideCursor() {
  std::fputs(ESC "?25l", TERM_STREAM);
}

inline void Utils::Term::showCursor() {
  std::fputs(ESC "?25h", TERM_STREAM);
}

inline void Utils::Term::scrollUp(const unsigned n) {
  std::fprintf(TERM_STREAM, ESC "%uS", n);
}

inline void Utils::Term::scrollDown(const unsigned n) {
  std::fprintf(TERM_STREAM, ESC "%uT", n);
}

inline void Utils::Term::eraseDisplay(const Clear clear) {
  std::fprintf(TERM_STREAM, ESC "%cJ", static_cast<uint8_t>(clear));
}

inline void Utils::Term::eraseLine(const Clear clear) {
  std::fprintf(TERM_STREAM, ESC "%cK", static_cast<uint8_t>(clear));
}

inline void Utils::Term::videoReset() {
  std::fputs(ESC "0m", TERM_STREAM);
}

inline void Utils::Term::intensity(const Intensity intensity) {
  switch (intensity) {
    case Intensity::NORMAL:
      std::fputs(ESC "22m", TERM_STREAM);
      break;
    case Intensity::BOLD:
      std::fputs(ESC "1m", TERM_STREAM);
      break;
    case Intensity::FAINT:
      std::fputs(ESC "2m", TERM_STREAM);
  }
}

inline void Utils::Term::italic(const bool on) {
  if (on) {
    std::fputs(ESC "3m", TERM_STREAM);
  } else {
    std::fputs(ESC "23m", TERM_STREAM);
  }
}

inline void Utils::Term::underline(const bool on) {
  if (on) {
    std::fputs(ESC "4m", TERM_STREAM);
  } else {
    std::fputs(ESC "24m", TERM_STREAM);
  }
}

inline void Utils::Term::blink(const Blink rate) {
  switch (rate) {
    case Blink::OFF:
      std::fputs(ESC "25m", TERM_STREAM);
      break;
    case Blink::SLOW:
      std::fputs(ESC "5m", TERM_STREAM);
      break;
    case Blink::FAST:
      std::fputs(ESC "6m", TERM_STREAM);
  }
}

inline void Utils::Term::videoNegative(const bool on) {
  if (on) {
    std::fputs(ESC "7m", TERM_STREAM);
  } else {
    std::fputs(ESC "27m", TERM_STREAM);
  }
}

inline void Utils::Term::conceal(const bool on) {
  if (on) {
    std::fputs(ESC "8m", TERM_STREAM);
  } else {
    std::fputs(ESC "28m", TERM_STREAM);
  }
}

inline void Utils::Term::strikethrough(const bool on) {
  if (on) {
    std::fputs(ESC "9m", TERM_STREAM);
  } else {
    std::fputs(ESC "29m", TERM_STREAM);
  }
}

inline void Utils::Term::primaryFont() {
  std::fputs(ESC "10m", TERM_STREAM);
}

inline void Utils::Term::alternativeFont(const unsigned n) {
  assert(n < 9);
  std::fprintf(TERM_STREAM, ESC "%um", 11u + n);
}

inline void Utils::Term::textColor(const Color color) {
  std::fprintf(TERM_STREAM, ESC "%um", 30u + static_cast<unsigned>(color));
}

inline void Utils::Term::textColor(const uint8_t r, const uint8_t g, const uint8_t b) {
  std::fprintf(TERM_STREAM, ESC "38;2;%c;%c;%cm", r, g, b);
}

inline void Utils::Term::defaultTextColor() {
  std::fputs(ESC "39m", TERM_STREAM);
}

inline void Utils::Term::backColor(const Color color) {
  std::fprintf(TERM_STREAM, ESC "%um", 40u + static_cast<unsigned>(color));
}

inline void Utils::Term::backColor(const uint8_t r, const uint8_t g, const uint8_t b) {
  std::fprintf(TERM_STREAM, ESC "48;2;%c;%c;%cm", r, g, b);
}

inline void Utils::Term::defaultBackColor() {
  std::fputs(ESC "49m", TERM_STREAM);
}

#undef ESC
#undef TERM_STREAM
