//
//  console color.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 24/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_console_color_hpp
#define engine_utils_console_color_hpp

#include <cstdint>
#include <cassert>
#include <iostream>
#include <string_view>

#define ESC "\x1B["
#define CONSTR constexpr std::string_view

namespace con {
  // commented constants aren't supported by the MacOS Terminal or iTerm2

  /// Reset all formatting states to their default values. Disable italic and
  /// underline, set text color and background colors to their defaults, etc.
  CONSTR reset         = ESC "0m";
  
  /// Either change to a bolder font, use a brighter text color or both
  CONSTR bold          = ESC "1m";
  /// Either change to a lighter font, use a dimmer text color or both
  CONSTR faint         = ESC "2m";
  /// Change font to an italic one [iTerm2 but not Terminal.app]
  CONSTR italic        = ESC "3m";
  /// Put a line underneath text
  CONSTR underline     = ESC "4m";
  /// Blink text slowly [Terminal.app but not iTerm2]
  CONSTR blink_slow    = ESC "5m";
  /// Set text color to background color and background color to text color
  CONSTR video_neg     = ESC "7m";
  /// Hide or "conceal" text [Terminal.app but not iTerm2]
  CONSTR conceal       = ESC "8m";
  
  /*
  CONSTR blink_fast    = ESC "6m";
  CONSTR strikethrough = ESC "9m";
  CONSTR primaryFont   = ESC "10m";
  CONSTR altFont[9] = {
    ESC "11m",
    ESC "12m",
    ESC "13m",
    ESC "14m",
    ESC "15m",
    ESC "16m",
    ESC "17m",
    ESC "18m",
    ESC "19m"
  };
  CONSTR fraktur       = ESC "20m";
  */
  
  /// Disable bold and faint modes
  CONSTR no_bold_faint = ESC "22m";
  /// Disable italic mode [iTerm2 but not Terminal.app]
  CONSTR no_italic     = ESC "23m";
  /// Disable underline mode
  CONSTR no_underline  = ESC "24m";
  /// Undo the effects of `video_neg`
  CONSTR video_pos     = ESC "27m";
  /// Undo the effects of `conceal` and show the text [Terminal.app but not iTerm2]
  CONSTR reveal        = ESC "28m";
  
  // CONSTR no_blink      = ESC "25m";
  // CONSTR no_strike     = ESC "29m";

  /// Set text color to black
  CONSTR text_black    = ESC "30m";
  /// Set text color to red
  CONSTR text_red      = ESC "31m";
  /// Set text color to green
  CONSTR text_green    = ESC "32m";
  /// Set text color to yellow
  CONSTR text_yellow   = ESC "33m";
  /// Set text color to blue
  CONSTR text_blue     = ESC "34m";
  /// Set text color to magenta
  CONSTR text_magenta  = ESC "35m";
  /// Set text color to cyan
  CONSTR text_cyan     = ESC "36m";
  /// Set text color to white
  CONSTR text_white    = ESC "37m";
  /// Set text color to its default
  CONSTR text_default  = ESC "39m";
  
  /// Set background color to black
  CONSTR back_black    = ESC "40m";
  /// Set background color to red
  CONSTR back_red      = ESC "41m";
  /// Set background color to green
  CONSTR back_green    = ESC "42m";
  /// Set background color to yellow
  CONSTR back_yellow   = ESC "43m";
  /// Set background color to blue
  CONSTR back_blue     = ESC "44m";
  /// Set background color to magenta
  CONSTR back_magenta  = ESC "45m";
  /// Set background color to cyan
  CONSTR back_cyan     = ESC "46m";
  /// Set background color to white
  CONSTR back_white    = ESC "47m";
  /// Set background color to its default
  CONSTR back_default  = ESC "49m";
  
  /*
  CONSTR frame         = ESC "51m";
  CONSTR encircle      = ESC "52m";
  CONSTR overline      = ESC "53m";
  CONSTR no_frame_circ = ESC "54m";
  CONSTR no_overline   = ESC "55m";
  */
  
  namespace detail {
    template <bool BACK>
    struct color_256 {
      explicit color_256(const unsigned index)
        : index{index} {
        assert(index < 256);
      }
      
    private:
      unsigned index;
      
      friend std::ostream &operator<<(std::ostream &stream, const color_256 color) {
        if constexpr (BACK) {
          stream << ESC "48;5;";
        } else {
          stream << ESC "38;5;";
        }
        return stream << color.index << 'm';
      }
    };
  }
  
  /// Set the text color using RGB values between 0-5. This is part of
  /// 256-color mode
  inline auto text_rgb_6(const unsigned r, const unsigned g, const unsigned b) {
    assert(r < 6);
    assert(g < 6);
    assert(b < 6);
    return detail::color_256<false>{16 + 36 * r + 6 * g + b};
  }
  
  /// Set the background color using RGB values between 0-5. This is part of
  /// 256-color mode
  inline auto back_rgb_6(const unsigned r, const unsigned g, const unsigned b) {
    assert(r < 6);
    assert(g < 6);
    assert(b < 6);
    return detail::color_256<true>{16 + 36 * r + 6 * g + b};
  }
  
  /// Set the text color to one of 24 shades of gray where 0 is black and 23 is
  /// white. This is part of 256-color mode
  inline auto text_gray_24(const unsigned gray) {
    assert(gray < 24);
    return detail::color_256<false>{232 + gray};
  }
  
  /// Set the background color to one of 24 shades of gray where 0 is black and
  /// 23 is white. This is part of 256-color mode
  inline auto back_gray_24(const unsigned gray) {
    assert(gray < 24);
    return detail::color_256<true>{232 + gray};
  }
  
  namespace detail {
    template <bool BACK>
    struct color_24bit {
      color_24bit(const unsigned r, const unsigned g, const unsigned b)
        : r{r}, g{g}, b{b} {}
    
    private:
      unsigned r, g, b;
      
      friend std::ostream &operator<<(std::ostream &stream, const color_24bit color) {
        if constexpr (BACK) {
          stream << ESC "48;2;";
        } else {
          stream << ESC "38;2;";
        }
        return stream << color.r << ';' << color.g << ';' << color.b << 'm';
      }
    };
  }
  
  /// Set the text color to one of 24 shades of gray where 0 is black and 23 is
  /// white. This is part of 24-bit color mode [iTerm2 but not Terminal.app]
  inline auto text_rgb_256(const unsigned r, const unsigned g, const unsigned b) {
    assert(r < 256);
    assert(g < 256);
    assert(b < 256);
    return detail::color_24bit<false>{r, g, b};
  }
  
  /// Set the background color using RGB values between 0-255. This is part of
  /// 24-bit color mode [iTerm2 but not Terminal.app]
  inline auto back_rgb_256(const unsigned r, const unsigned g, const unsigned b) {
    assert(r < 256);
    assert(g < 256);
    assert(b < 256);
    return detail::color_24bit<true>{r, g, b};
  }
}

#undef CONSTR
#undef ESC

#endif
