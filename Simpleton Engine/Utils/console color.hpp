//
//  console color.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 24/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_console_color_hpp
#define engine_utils_console_color_hpp

#include <string_view>

#define ESC "\x1B["
#define CONSTR constexpr std::string_view

namespace con {
  // commented constants aren't supported by the MacOS Terminal or iTerm2

  CONSTR reset         = ESC "0m";
  
  CONSTR bold          = ESC "1m";
  CONSTR faint         = ESC "2m";
  CONSTR italic        = ESC "3m"; // iTerm2 but not Terminal.app
  CONSTR underline     = ESC "4m";
  CONSTR blink_slow    = ESC "5m"; // Terminal.app but not iTerm2
  // CONSTR blink_fast    = ESC "6m";
  CONSTR video_neg     = ESC "7m";
  CONSTR conceal       = ESC "8m"; // Terminal.app but not iTerm2
  /*
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
  // bold: off or underline: double 21m
  */
  
  CONSTR no_bold_faint = ESC "22m";
  CONSTR no_italic     = ESC "23m"; // iTerm2 but not Terminal.app
  CONSTR no_underline  = ESC "24m";
  // CONSTR no_blink      = ESC "25m";
  // reserved 26m
  CONSTR video_pos     = ESC "27m";
  CONSTR reveal        = ESC "28m"; // Terminal.app but not iTerm2
  // CONSTR no_strike     = ESC "29m";

  CONSTR text_black    = ESC "30m";
  CONSTR text_red      = ESC "31m";
  CONSTR text_green    = ESC "32m";
  CONSTR text_yellow   = ESC "33m";
  CONSTR text_blue     = ESC "34m";
  CONSTR text_magenta  = ESC "35m";
  CONSTR text_cyan     = ESC "36m";
  CONSTR text_white    = ESC "37m";
  // extended text color 38;2;r;g;bm
  CONSTR text_default  = ESC "39m";
  
  CONSTR back_black    = ESC "40m";
  CONSTR back_red      = ESC "41m";
  CONSTR back_green    = ESC "42m";
  CONSTR back_yellow   = ESC "43m";
  CONSTR back_blue     = ESC "44m";
  CONSTR back_magenta  = ESC "45m";
  CONSTR back_cyan     = ESC "46m";
  CONSTR back_white    = ESC "47m";
  // extended back color 48;2;r;g;bm
  CONSTR back_default  = ESC "49m";
  
  // reserved 50m
  /*
  CONSTR frame         = ESC "51m";
  CONSTR encircle      = ESC "52m";
  CONSTR overline      = ESC "53m";
  CONSTR no_frame_circ = ESC "54m";
  CONSTR no_overline   = ESC "55m";
  */
}

#undef CONSTR
#undef ESC

#endif
