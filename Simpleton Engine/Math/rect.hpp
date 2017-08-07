//
//  rect.hpp
//  Game Engine
//
//  Created by Indi Kernick on 28/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_math_rect_hpp
#define engine_math_rect_hpp

#include <cmath>
#include "dir.hpp"
#include <iostream>
#include "scale.hpp"
#include <glm/vec2.hpp>

namespace Math {
  template <typename T>
  struct RectPS;
  
  template <typename T, Math::Dir PLUS_X, Math::Dir PLUS_Y>
  struct RectCS;

  ///A rectangle defined by a top-left point and a bottom-right point
  template <typename T>
  struct RectPP {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    
    using Scalar = T;
    using Vector = glm::tvec2<Scalar>;
  
    static constexpr Scalar EPSILON = std::is_integral<Scalar>::value ? Scalar(1) : Scalar(0);
    static const RectPP LARGEST;
    
    RectPP()
      : tl(Scalar(0), Scalar(0)), br(-EPSILON, -EPSILON) {}
    RectPP(const RectPP &) = default;
    RectPP(RectPP &&) = default;
    ~RectPP() = default;
    
    RectPP &operator=(const RectPP &) = default;
    RectPP &operator=(RectPP &&) = default;
    
    explicit RectPP(const Vector size)
      : tl(Scalar(0), Scalar(0)), br(size - EPSILON) {}
    RectPP(const Vector tl, const Vector br)
      : tl(tl), br(br) {}
    RectPP(const Scalar left, const Scalar top, const Scalar right, const Scalar bottom)
      : tl(left, top), br(right, bottom) {}
    
    template <typename U>
    explicit RectPP(const RectPP<U> other)
      : tl(static_cast<Scalar>(other.tl.x),
           static_cast<Scalar>(other.tl.y)),
        br(static_cast<Scalar>(other.br.x + RectPP<U>::EPSILON) - EPSILON,
           static_cast<Scalar>(other.br.y + RectPP<U>::EPSILON) - EPSILON) {}
    
    template <typename U>
    explicit RectPP(const RectPS<U> other)
      : tl(static_cast<Scalar>(other.p.x),
           static_cast<Scalar>(other.p.y)),
        br(static_cast<Scalar>(other.p.x + other.s.x) - EPSILON,
           static_cast<Scalar>(other.p.y + other.s.y) - EPSILON) {}
    
    bool operator==(const RectPP other) const {
      return tl == other.tl && br == other.br;
    }
    bool operator!=(const RectPP other) const {
      return tl != other.tl || br != other.br;
    }
    
    Scalar side(const Math::Dir dir) const {
      switch (dir) {
        case Math::Dir::TOP:
          return top;
        case Math::Dir::RIGHT:
          return right;
        case Math::Dir::BOTTOM:
          return bottom;
        case Math::Dir::LEFT:
          return left;
        default:
          assert(false);
      }
    }
    
    void side(const Math::Dir dir, const Scalar val) {
      switch (dir) {
        case Math::Dir::TOP:
          top = val;
          break;
        case Math::Dir::RIGHT:
          right = val;
          break;
        case Math::Dir::BOTTOM:
          bottom = val;
          break;
        case Math::Dir::LEFT:
          left = val;
          break;
        default:
          assert(false);
      }
    }
    
    Vector topLeft() const {
      return tl;
    }
    Vector topRight() const {
      return {br.x, tl.y};
    }
    Vector bottomRight() const {
      return br;
    }
    Vector bottomLeft() const {
      return {tl.x, br.y};
    }
    
    void topLeft(const Vector val) {
      tl = val;
    }
    void topRight(const Vector val) {
      br.x = val.x;
      tl.y = val.y;
    }
    void bottomRight(const Vector val) {
      br = val;
    }
    void bottomLeft(const Vector val) {
      tl.x = val.x;
      br.y = val.y;
    }
    
    Vector size() const {
      return (br - tl) + EPSILON;
    }
    
    bool sizeZero() const {
      return (br.x - tl.x) + EPSILON == Scalar(0) &&
             (br.y - tl.y) + EPSILON == Scalar(0);
    }
    
    ///Enlarge the rectangle to enclose the point
    void extendToEnclose(const Vector point) {
      if (point.x < tl.x) {
        tl.x = point.x;
      } else if (point.x > br.x) {
        br.x = point.x;
      }
      
      if (point.y < tl.y) {
        tl.y = point.y;
      } else if (point.y > br.y) {
        br.y = point.y;
      }
    }
    
    bool interceptsWith(const RectPP other) const {
      return tl.x           < other.br.x + EPSILON &&
             tl.y           < other.br.y + EPSILON &&
             br.x + EPSILON > other.tl.x           &&
             br.y + EPSILON > other.tl.y;
    }
    bool encloses(const RectPP other) const {
      return tl.x <= other.tl.x &&
             tl.y <= other.tl.y &&
             br.x >= other.br.x &&
             br.y >= other.br.y;
    }
    bool encloses(const Vector other) const {
      return tl.x <= other.x &&
             tl.y <= other.y &&
             br.x >= other.x &&
             br.y >= other.y;
    }
    
    friend std::ostream &operator<<(std::ostream &stream, const RectPP rect) {
      return stream << "RectPP {{" << rect.left << ", " << rect.top << "}, {" << rect.right << ", " << rect.bottom << "}}";
    }
    
    #pragma clang diagnostic push 
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic ignored "-Wnested-anon-types"
  
    union {
      struct {
        //top left corner
        Vector tl;
        //bottom right corner
        Vector br;
      };
      struct {
        Scalar left;
        Scalar top;
        Scalar right;
        Scalar bottom;
      };
    };
    
    #pragma clang diagnostic pop
  };
  
  template <typename T>
  const RectPP<T> RectPP<T>::LARGEST = {
    std::numeric_limits<T>::lowest(),
    std::numeric_limits<T>::lowest(),
    std::numeric_limits<T>::max(),
    std::numeric_limits<T>::max()
  };
  
  ///A rectangle defined by a top-left point and a size
  template <typename T>
  struct RectPS {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
  
    using Scalar = T;
    using Vector = glm::tvec2<Scalar>;
  
    static constexpr Scalar EPSILON = std::is_integral<Scalar>::value ? Scalar(1) : Scalar(0);
    static const RectPS LARGEST;
  
    RectPS() = default;
    RectPS(const RectPS &) = default;
    RectPS(RectPS &&) = default;
    ~RectPS() = default;
    
    RectPS &operator=(const RectPS &) = default;
    RectPS &operator=(RectPS &&) = default;
    
    explicit RectPS(const Vector size)
      : p(T(0), T(0)), s(size) {}
    RectPS(const Vector pos, const Vector size)
      : p(pos), s(size) {}
    RectPS(const Scalar px, const Scalar py, const Scalar sx, const Scalar sy)
      : p(px, py), s(sx, sy) {}
    
    template <typename U>
    explicit RectPS(const RectPS<U> other)
      : p(static_cast<T>(other.p.x), static_cast<T>(other.p.y)),
        s(static_cast<T>(other.s.x), static_cast<T>(other.s.y)) {}
    
    template <typename U>
    explicit RectPS(const RectPP<U> other)
      : p(static_cast<T>(other.tl.x),
          static_cast<T>(other.tl.y)),
        s(static_cast<T>(other.br.x - other.tl.x + RectPP<U>::EPSILON),
          static_cast<T>(other.br.y - other.tl.y + RectPP<U>::EPSILON)) {}
    
    bool operator==(const RectPS other) const {
      return p == other.p && s == other.s;
    }
    bool operator!=(const RectPS other) const {
      return p != other.p || s != other.s;
    }
    
    Scalar left() const {
      return p.x;
    }
    Scalar top() const {
      return p.y;
    }
    Scalar right() const {
      return p.x + s.x - EPSILON;
    }
    Scalar bottom() const {
      return p.y + s.y - EPSILON;
    }
    
    void left(const Scalar val) {
      p.x = val;
    }
    void top(const Scalar val) {
      p.y = val;
    }
    void right(const Scalar val) {
      s.x = val - p.x + EPSILON;
    }
    void bottom(const Scalar val) {
      s.y = val - p.y + EPSILON;
    }
    
    Scalar side(const Math::Dir dir) const {
      switch (dir) {
        case Math::Dir::TOP:
          return top();
        case Math::Dir::RIGHT:
          return right();
        case Math::Dir::BOTTOM:
          return bottom();
        case Math::Dir::LEFT:
          return left();
        default:
          assert(false);
      }
    }
    
    void side(const Math::Dir dir, const Scalar val) {
      switch (dir) {
        case Math::Dir::TOP:
          top(val);
          break;
        case Math::Dir::RIGHT:
          right(val);
          break;
        case Math::Dir::BOTTOM:
          bottom(val);
          break;
        case Math::Dir::LEFT:
          left(val);
          break;
        default:
          assert(false);
      }
    }
    
    bool interceptsWith(const RectPS other) const {
      return p.x       < other.p.x + other.s.x &&
             p.y       < other.p.y + other.s.y &&
             p.x + s.x > other.p.x             &&
             p.y + s.y > other.p.y;
    }
    bool encloses(const RectPS other) const {
      return p.x       <= other.p.x             &&
             p.y       <= other.p.y             &&
             p.x + s.x >= other.p.x + other.s.x &&
             p.y + s.y >= other.p.y + other.s.y;
    }
    bool encloses(const Vector other) const {
      return p.x                 <= other.x &&
             p.y                 <= other.y &&
             p.x + s.x - EPSILON >= other.x &&
             p.y + s.y - EPSILON >= other.y;
    }
    
    friend std::ostream &operator<<(std::ostream &stream, const RectPS rect) {
      return stream << "RectPS {{" << rect.p.x << ", " << rect.p.y << "}, {" << rect.s.x << ", " << rect.s.y << "}}";
    }
    
    Vector p;
    Vector s;
  };
  
  template <typename T>
  const RectPS<T> RectPS<T>::LARGEST = {
    {
      std::numeric_limits<T>::lowest(),
      std::numeric_limits<T>::lowest()
    },
    {
      std::numeric_limits<T>::max(),
      std::numeric_limits<T>::max()
    }
  };
  
  ///A rectangle defined by a center and a size
  template <typename T, Math::Dir PLUS_X, Math::Dir PLUS_Y>
  struct RectCS {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    
    using Scalar = T;
    using Vector = glm::tvec2<Scalar>;
    
    static constexpr Scalar EPSILON = std::is_integral<Scalar>::value ? Scalar(1) : Scalar(0);
    
    RectCS() = default;
    RectCS(const RectCS &) = default;
    RectCS(RectCS &&) = default;
    ~RectCS() = default;
    
    RectCS &operator=(const RectCS &) = default;
    RectCS &operator=(RectCS &&) = default;
    
    explicit RectCS(const Vector size)
      : c(Scalar(0), Scalar(0)), s(size) {}
    RectCS(const Vector center, const Vector size)
      : c(center), s(size) {}
    RectCS(const Scalar cx, const Scalar cy, const Scalar sx, const Scalar sy)
      : c(cx, cy), s(sx, sy) {}
    
    bool operator==(const RectCS other) const {
      return c == other.c && s == other.s;
    }
    bool operator!=(const RectCS other) const {
      return c != other.c || s != other.s;
    }
    
    Scalar side(const Math::Dir dir) const {
      //@TODO There are probably some off-by-one errors in side getter and setter
    
      switch (dir) {
        case PLUS_Y:
          return c.y + s.y / Scalar(2);
        case PLUS_X:
          return c.x + s.x / Scalar(2);
        case opposite(PLUS_Y):
          return c.y - s.y / Scalar(2);
        case opposite(PLUS_X):
          return c.x - s.x / Scalar(2);
        default:
          assert(false);
      }
    }
    
    void side(const Math::Dir dir, const Scalar val) {
      switch (dir) {
        case PLUS_Y: {
          const float delta = val - (c.y + s.y / Scalar(2));
          c.y += delta / Scalar(2);
          s.y += delta;
          break;
        }
        case PLUS_X: {
          const float delta = val - (c.x + s.x / Scalar(2));
          c.x += delta / Scalar(2);
          s.x += delta;
          break;
        }
        case opposite(PLUS_Y): {
          const float delta = val - (c.y - s.y / Scalar(2));
          c.y += delta / Scalar(2);
          s.y += delta;
          break;
        }
        case opposite(PLUS_X): {
          const float delta = val - (c.x - s.x / Scalar(2));
          c.x += delta / Scalar(2);
          s.x += delta;
          break;
        }
        default:
          assert(false);
      }
    }
    
    Scalar top() const {
      return side(Math::Dir::TOP);
    }
    Scalar right() const {
      return side(Math::Dir::RIGHT);
    }
    Scalar bottom() const {
      return side(Math::Dir::BOTTOM);
    }
    Scalar left() const {
      return side(Math::Dir::LEFT);
    }
    
    void top(const Scalar val) {
      side(Math::Dir::TOP, val);
    }
    void right(const Scalar val) {
      side(Math::Dir::RIGHT, val);
    }
    void bottom(const Scalar val) {
      side(Math::Dir::BOTTOM, val);
    }
    void left(const Scalar val) {
      side(Math::Dir::LEFT, val);
    }
    
    //@TODO check for off-by-one errors
    bool interceptsWith(const RectCS other) const {
      return std::abs(c.x - other.c.x) < s.x / Scalar(2) + other.s.x / Scalar(2) &&
             std::abs(c.y - other.c.y) < s.y / Scalar(2) + other.s.y / Scalar(2);
    }
    bool encloses(const RectCS other) const {
      return std::abs(c.x - other.c.x) < (s.x - other.s.x) / Scalar(2) &&
             std::abs(c.y - other.c.y) < (s.y - other.s.y) / Scalar(2);
    }
    bool encloses(const Vector other) const {
      return std::abs(c.x - other.x) < s.x / Scalar(2) &&
             std::abs(c.y - other.y) < s.y / Scalar(2);
    }
    
    union {
      Vector c;
      Vector center;
    };
    union {
      Vector s;
      Vector size;
    };
  };
}
#endif
