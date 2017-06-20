//
//  safe down cast.hpp
//  Game Engine
//
//  Created by Indi Kernick on 15/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_safe_down_cast_hpp
#define engine_utils_safe_down_cast_hpp

#include <memory>

#ifdef RELEASE

namespace Utils {
  ///Dynamic cast and return nullptr on failure
  template <typename Derived, typename Base>
  std::enable_if_t<
    std::is_base_of<Base, Derived>::value &&
    !std::is_same<Base, Derived>::value,
    Derived *
  >
  safeDownCast(Base * const base) {
    return static_cast<Derived * const>(base);
  }

  ///Dynamic cast and return nullptr on failure
  template <typename Derived, typename Base>
  std::enable_if_t<
    std::is_base_of<Base, Derived>::value &&
    !std::is_same<Base, Derived>::value,
    const Derived *
  >
  safeDownCast(const Base * const base) {
    return static_cast<const Derived * const>(base);
  }

  ///Dynamic cast and return nullptr on failure
  template <typename Derived, typename Base>
  std::enable_if_t<
    std::is_base_of<Base, Derived>::value &&
    !std::is_same<Base, Derived>::value,
    std::shared_ptr<Derived>
  >
  safeDownCast(const std::shared_ptr<Base> &base) {
    return std::static_pointer_cast<Derived>(base);
  }

  ///Dynamic cast and return nullptr on failure
  template <typename Derived, typename Base>
  std::enable_if_t<
    std::is_base_of<Base, Derived>::value &&
    !std::is_same<Base, Derived>::value,
    std::shared_ptr<const Derived>
  >
  safeDownCast(const std::shared_ptr<const Base> &base) {
    return std::static_pointer_cast<const Derived>(base);
  }
}

#else

namespace Utils {
  ///Dynamic cast and throw std::bad_cast on failure
  template <typename Derived, typename Base>
  inline std::enable_if_t<
    std::is_base_of<Base, Derived>::value &&
    !std::is_same<Base, Derived>::value,
    Derived *
  >
  safeDownCast(Base * const base) {
    Derived * const derived = dynamic_cast<Derived * const>(base);
    if (derived == nullptr) {
      throw std::bad_cast();
    } else {
      return derived;
    }
  }

  ///Dynamic cast and throw std::bad_cast on failure
  template <typename Derived, typename Base>
  std::enable_if_t<
    std::is_base_of<Base, Derived>::value &&
    !std::is_same<Base, Derived>::value,
    const Derived *
  >
  safeDownCast(const Base * const base) {
    const Derived * const derived = dynamic_cast<const Derived * const>(base);
    if (derived == nullptr) {
      throw std::bad_cast();
    } else {
      return derived;
    }
  }

  ///Dynamic cast and throw std::bad_cast on failure
  template <typename Derived, typename Base>
  std::enable_if_t<
    std::is_base_of<Base, Derived>::value &&
    !std::is_same<Base, Derived>::value,
    std::shared_ptr<Derived>
  >
  safeDownCast(const std::shared_ptr<Base> &base) {
    Derived * const derived = dynamic_cast<Derived * const>(base.get());
    if (derived == nullptr) {
      throw std::bad_cast();
    } else {
      return std::shared_ptr<Derived>(base, derived);
    }
  }

  ///Dynamic cast and throw std::bad_cast on failure
  template <typename Derived, typename Base>
  std::enable_if_t<
    std::is_base_of<Base, Derived>::value &&
    !std::is_same<Base, Derived>::value,
    std::shared_ptr<const Derived>
  >
  safeDownCast(const std::shared_ptr<const Base> &base) {
    const Derived * const derived = dynamic_cast<const Derived * const>(base.get());
    if (derived == nullptr) {
      throw std::bad_cast();
    } else {
      return std::shared_ptr<const Derived>(base, derived);
    }
  }
}

#endif

namespace Utils {
  ///Lock a std::weak_ptr and throw std::bad_weak_ptr on failure
  template <typename T>
  std::shared_ptr<T> safeLock(const std::weak_ptr<T> &weak) {
    return std::shared_ptr<T>(weak);
  }
}

#endif
