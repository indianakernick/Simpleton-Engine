//
//  instance limiter.hpp
//  Game Engine
//
//  Created by Indi Kernick on 8/10/2016.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_instance_limiter_hpp
#define engine_utils_instance_limiter_hpp

#include <string>
#include <stdexcept>
#include <experimental/string_view>

namespace Utils {
  class TooManyInstances final : std::logic_error {
  public:
    TooManyInstances(std::experimental::string_view, size_t);
  };
}

#ifdef ENABLE_INSTANCE_LIMITER

#include "type name.hpp"

namespace Utils {
  ///Limit the number of instances of a derived class
  template <typename T, size_t MAX_COUNT>
  class LimitInstances {
  private:
    static void checkCount() {
      if (count > MAX_COUNT) {
        throw TooManyInstances(typeName<T>(), MAX_COUNT);
      }
    }

  protected:
    LimitInstances() {
      count++;
      checkCount();
    }
    LimitInstances(const LimitInstances &) {
      count++;
      checkCount();
    }
    LimitInstances(LimitInstances &&) {
      count++;
    }
    ~LimitInstances() {
      static_assert(std::is_base_of<LimitInstances, T>::value);
      count--;
    }
    
    LimitInstances &operator=(const LimitInstances &) = default;
    LimitInstances &operator=(LimitInstances &&) = default;

  private:
    static size_t count;
  };

  template <typename T, size_t MAX_COUNT>
  size_t LimitInstances<T, MAX_COUNT>::count = 0;

  ///Limit the number of instances of a derived class
  template <typename T>
  class LimitInstances<T, 1> {
  protected:
    LimitInstances() {
      if (created) {
        throw TooManyInstances(typeName<T>(), 1);
      } else {
        created = true;
      }
    }
    LimitInstances(const LimitInstances &) = delete;
    LimitInstances(LimitInstances &&) = default;
    ~LimitInstances() {
      static_assert(std::is_base_of<LimitInstances, T>::value);
      created = false;
    }
    
    LimitInstances &operator=(const LimitInstances &) = delete;
    LimitInstances &operator=(LimitInstances &&) = default;

  public:
    static bool wasCreated() {
      return created;
    }
    
  private:
    static bool created;
  };

  template <typename T>
  bool LimitInstances<T, 1>::created = false;
}

#else

namespace Utils {
  ///Limit the number of instances of a derived class
  template <typename T, size_t>
  class LimitInstances {
  protected:
    LimitInstances() = default;
    LimitInstances(const LimitInstances &) = default;
    LimitInstances(LimitInstances &&) = default;
    ~LimitInstances() {
      static_assert(std::is_base_of<LimitInstances, T>::value);
    }
    
    LimitInstances &operator=(const LimitInstances &) = default;
    LimitInstances &operator=(LimitInstances &&) = default;
  };

  ///Limit the number of instances of a derived class
  template <typename T>
  class LimitInstances<T, 1> {
  protected:
    LimitInstances() = default;
    LimitInstances(const LimitInstances &) = delete;
    LimitInstances(LimitInstances &&) = default;
    ~LimitInstances() {
      static_assert(std::is_base_of<LimitInstances, T>::value);
    }
    
    LimitInstances &operator=(const LimitInstances &) = delete;
    LimitInstances &operator=(LimitInstances &&) = default;
  
  public:
    static bool wasCreated() {
      return true;
    }
  };
}

#endif

namespace Utils {
  //I don't think there is any point in this
  
  ///Limit the number of instances of a derived class
  template <typename T>
  class LimitInstances<T, 0> {
  protected:
    LimitInstances() = delete;
    LimitInstances(const LimitInstances &) = delete;
    LimitInstances(LimitInstances &&) = delete;
    ~LimitInstances() = delete;
    
    LimitInstances &operator=(const LimitInstances &) = delete;
    LimitInstances &operator=(LimitInstances &&) = delete;
  };

  ///Force a derived class to be a singleton
  template <typename T>
  using ForceSingleton = LimitInstances<T, 1>;
}

#endif
