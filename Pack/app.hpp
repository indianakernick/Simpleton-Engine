//
//  app.hpp
//  Pack
//
//  Created by Indi Kernick on 24/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef app_hpp
#define app_hpp

#include <exception>

class ArgError final : public std::exception {
public:
  ArgError() = default;
};

void printUsage();
void runApp(int, const char **);

#endif
