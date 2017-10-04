//
//  system info.hpp
//  Game Engine
//
//  Created by Indi Kernick on 5/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_platform_system_info_hpp
#define engine_platform_system_info_hpp

#include <string>

namespace Platform {
  class ResPathError final : std::logic_error {
  public:
    explicit ResPathError(const char *);
  };

  std::string getSaveDir(const std::string &companyName, const std::string &appName);
  std::string getResDir();
}

#endif
