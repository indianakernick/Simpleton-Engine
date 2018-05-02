//
//  sheet tex.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 2/5/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_sheet_tex_hpp
#define engine_graphics_2d_sheet_tex_hpp

#include <Simpleton/Sprite/sheet.hpp>
#include <Simpleton/Graphics 2D/renderer.hpp>

namespace G2D {
  class SheetTex {
  public:
    void load(G2D::Renderer &, const std::string &, GLenum = GL_NEAREST);
    const Sprite::Sheet &sheet() const;
    G2D::TextureID tex() const;
    
  private:
    Sprite::Sheet mSheet;
    G2D::TextureID mTex;
  };
}

#include "sheet tex.inl"

#endif
