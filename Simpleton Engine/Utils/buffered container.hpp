//
//  buffered container.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 23/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_buffered_container_hpp
#define engine_utils_buffered_container_hpp

#include <vector>

//@TODO Think of a better name

namespace Utils {
  class ModificationError : std::logic_error {
  public:
    inline ModificationError()
      : std::logic_error("Container modification error") {}
  };

  template <typename Container>
  class BufferedMapContainer {
  public:
    
    template <typename ...Args>
    void add(Args &&... args) {
      if (modifying) {
        addedElements.emplace_back(std::forward<Args>(args)...);
      } else {
        container.emplace(std::forward<Args>(args)...);
      }
    }
    
    void rem(const typename Container::key_type key) {
      if (modifying) {
        removedElements.emplace_back(container.find(key));
      } else {
        container.erase(key);
      }
    }
    
    const Container &view() const {
      return container;
    }
    
    Container &startModifying() {
      if (modifying) {
        throw ModificationError();
      }
      modifying = true;
      return container;
    }
    
    void stopModifying() {
      if (!modifying) {
        throw ModificationError();
      }
      modifying = false;
      
      for (auto e = removedElements.cbegin(); e != removedElements.cbegin(); ++e) {
        container.erase(*e);
      }
      removedElements.clear();
      
      for (auto e = addedElements.cbegin(); e != addedElements.cend(); ++e) {
        container.emplace(std::move(*e));
      }
      addedElements.clear();
    }
  
  private:
    Container container;
    std::vector<typename Container::iterator> removedElements;
    std::vector<typename Container::value_type> addedElements;
    bool modifying = false;
  };
}

#endif
