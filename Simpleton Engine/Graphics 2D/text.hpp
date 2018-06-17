//
//  text.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 17/6/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_text_hpp
#define engine_graphics_2d_text_hpp

#include <string_view>
#include <type_traits>
#include "quad writer.hpp"

namespace G2D {
  enum class Align {
    LEFT,
    CENTER,
    RIGHT
  };

  class Text {
  
    template <typename T, typename Ret>
    using EnableNotStr = std::enable_if_t<
      !std::is_convertible_v<T, std::string_view>,
      Ret
    >;

  public:
    Text() = default;
    explicit Text(Section &);
    
    void section(Section &);
    Section &section() const;
    
    void glyphSize(glm::vec2);
    void advance(glm::vec2);
    void scale(float);
    void depth(float);
    template <typename Enum>
    void depth(Enum);
    
    template <Align ALIGN = Align::LEFT, PlusXY PLUS_XY = PlusXY::RIGHT_DOWN>
    glm::vec2 write(glm::vec2, std::string_view);
    template <Align ALIGN = Align::LEFT, PlusXY PLUS_XY = PlusXY::RIGHT_DOWN>
    glm::vec2 write(glm::vec2, char);
    template <Align ALIGN = Align::LEFT, PlusXY PLUS_XY = PlusXY::RIGHT_DOWN, typename T>
    EnableNotStr<T, glm::vec2> write(glm::vec2, const T &);
    
  private:
    Section *section_ = nullptr;
    glm::vec2 glyphSize_ {1.0f};
    glm::vec2 advance_ {1.0f};
    float scale_ = 1.0f;
    float depth_ = 0.0f;

    template <PlusXY PLUS_XY>
    glm::vec2 writeLeft(glm::vec2, std::string_view);
    template <PlusXY PLUS_XY>
    void writeChar(glm::vec2, glm::vec2, char);
  };
}

#include "text.inl"

#endif
