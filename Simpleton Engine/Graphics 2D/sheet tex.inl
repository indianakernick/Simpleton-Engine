//
//  sheet tex.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 2/5/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include <Simpleton/SDL/paths.hpp>

inline void G2D::SheetTex::load(
  G2D::Renderer &renderer,
  const std::string &fontname,
  const GLenum filter
) {
  GL::TexParams2D params;
  params.setWrap(GL_CLAMP_TO_EDGE);
  params.setFilter(filter);
  const std::string path = SDL::res(fontname);
  mTex = renderer.addTexture(path + ".png", params);
  mSheet = Sprite::makeSheet(path + ".atlas");
}

inline const Sprite::Sheet &G2D::SheetTex::sheet() const {
  return mSheet;
}

inline G2D::TextureID G2D::SheetTex::tex() const {
  return mTex;
}
