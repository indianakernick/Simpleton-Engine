//
//  sheet writer.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 2/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_sheet_writer_hpp
#define engine_graphics_2d_sheet_writer_hpp

#include "quad writer.hpp"
#include "../Sprite/sheet.hpp"

namespace G2D {
  /// A reference to a Sprite::Sheet and a reference to a QuadWriter
  struct SheetWriter {
    const Sprite::Sheet &sheet;
    QuadWriter &writer;
  };
}

#endif
