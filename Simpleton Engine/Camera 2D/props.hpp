//
//  props.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 15/11/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_camera_2d_props_hpp
#define engine_camera_2d_props_hpp

#include <glm/vec2.hpp>

namespace Cam2D {
  struct Props {
    ///Position in meters of the camera
    glm::vec2 pos;
    ///Scale factor (zoom)
    float scale;
    ///Aspect ratio of the window
    float aspect;
  };
  
  enum class PropID {
    POS,
    ZOOM
  };
  
  template <PropID>
  struct PropTraits;
  
  template <>
  struct PropTraits<PropID::POS> {
    using type = glm::vec2;
    static constexpr auto PTR = &Props::pos;
  };
  
  template <>
  struct PropTraits<PropID::ZOOM> {
    using type = float;
    static constexpr auto PTR = &Props::scale;
  };
  
  template <PropID PROP>
  using PropType = typename PropTraits<PROP>::type;
  
  template <PropID PROP>
  constexpr PropType<PROP> &getProp(Props &props) {
    return props.*PropTraits<PROP>::PTR;
  }
  
  template <PropID PROP>
  constexpr const PropType<PROP> &getProp(const Props &props) {
    return props.*PropTraits<PROP>::PTR;
  }
}

#endif
