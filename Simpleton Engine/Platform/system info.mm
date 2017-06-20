//
//  system info.mm
//  Game Engine
//
//  Created by Indi Kernick on 11/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "system info.hpp"

#import <Foundation/Foundation.h>

std::string Platform::getResDir() {
  if (NSBundle *bundle = [NSBundle mainBundle]; bundle == nil) {
    throw ResPathError("Bundle is missing");
  } else {
    if (NSString *resPath = [bundle resourcePath]; resPath == nil) {
      throw ResPathError("Bundle doesn't have a Resources folder");
    } else {
      return [resPath cStringUsingEncoding:(NSASCIIStringEncoding)] + std::string("/");
    }
  }
}
