//
//  keyframe animation.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 9/10/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_keyframe_animation_hpp
#define engine_graphics_keyframe_animation_hpp

#include <vector>
#include "../Utils/tuple.hpp"
#include "../Math/interpolate.hpp"

namespace Graphics {
  template <typename DataT>
  struct Keyframe {
    using Data = DataT;
    
    Keyframe(const float offset, const Data &data)
      : data(data), offset(offset) {}
    Keyframe(const float offset, Data &&data)
      : data(std::move(data)), offset(offset) {}
    
    Data data;
    float offset;
  };

  template <typename DataT>
  struct Animation {
    using Data = DataT;
    using Keyframes = std::vector<Keyframe<DataT>>;
    
    Animation(const float duration, const size_t size)
      : keyframes(size), duration(duration) {}
    Animation(const float duration, const Keyframes &keyframes)
      : keyframes(keyframes), duration(duration) {}
    Animation(const float duration, Keyframes &&keyframes)
      : keyframes(std::move(keyframes)), duration(duration) {}
    
    Keyframes keyframes;
    float duration;
  };

  template <typename Number>
  struct Interpolator {
    template <typename Num>
    static std::enable_if_t<
      !Utils::IsTupleLike<Number>::value
      && std::is_same<Num, Number>::value
    >
    lerp(
      Num &dst,
      const float t,
      const Num min,
      const Num max
    ) {
      dst = Math::lerp(t, min, max);
    }
    
    template <typename Num>
    static std::enable_if_t<
      Utils::IsTupleLike<Number>::value
      && std::is_same<Num, Number>::value
    >
    lerp(
      Num &dst,
      const float t,
      const Num &min,
      const Num &max
    ) {
      constexpr size_t size = std::tuple_size<Number>::value;
      Utils::forEachIndex<size>([&dst, t, &min, &max] (auto i) {
        constexpr size_t index = UTILS_VALUE(i);
        using ElemType = std::tuple_element_t<index, decltype(dst)>;
        std::get<index>(dst) = Interpolator<ElemType>::lerp(
          t,
          std::get<index>(min),
          std::get<index>(max)
        );
      });
    }
  };

  template <typename Number>
  struct Interpolator<std::vector<Number>> {
    static void lerp(
      std::vector<Number> &dst,
      const float t,
      const std::vector<Number> &min,
      const std::vector<Number> &max
    ) {
      if (min.size() != max.size()) {
        throw std::runtime_error("Cannot interpolate vector's of different sizes");
      }
      if (dst.size() != min.size()) {
        dst.resize(min.size());
      }
      for (size_t i = 0; i != min.size(); ++i) {
        Interpolator<Number>::lerp(dst[i], t, min[i], max[i]);
      }
    }
  };

  template <typename Data>
  void lerpKeyframe(
    Data &dst,
    const float t,
    const Keyframe<Data> &min,
    const Keyframe<Data> &max
  ) {
    using Lerp = Interpolator<Data>;
    Lerp::lerp(
      dst,
      Math::invLerp<float>(t, min.offset, max.offset),
      min.data,
      max.data
    );
  }

  template <typename Data>
  void lerpAnimation(
    Data &dst,
    const float t,
    const Animation<Data> &anim
  ) {
    const size_t size = anim.keyframes.size();
    
    if (size == 0) {
      throw std::runtime_error("Cannot interpolate an animation with no keyframes");
    } else if (size == 1) {
      dst = anim.keyframes[0].data;
      return;
    } else if (size == 2) {
      return lerpKeyframe(dst, t, anim.keyframes[0], anim.keyframes[1]);
    }
    
    if (t < anim.keyframes[0].offset) {
      return lerpKeyframe(dst, t, anim.keyframes[0], anim.keyframes[1]);
    } else if (t > anim.keyframes[size - 1].offset) {
      return lerpKeyframe(dst, t, anim.keyframes[size - 2], anim.keyframes[size - 1]);
    }
    
    //std::lower_bound maybe?
    for (size_t k = 1; k != size; ++k) {
      if (anim.keyframes[k].offset >= t) {
        return lerpKeyframe(dst, t, anim.keyframes[k - 1], anim.keyframes[k]);
      }
    }
    
    throw std::runtime_error("Keyframes must be sorted (in ascending order) by their offsets");
  }
}

#endif
