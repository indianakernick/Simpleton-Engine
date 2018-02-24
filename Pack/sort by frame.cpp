//
//  sort by frame.cpp
//  Pack
//
//  Created by Indi Kernick on 17/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "sort by frame.hpp"

#include <cerrno>

namespace {
  int parseInt(const char *begin, const char *) {
    /*
    @TODO
    
    int number;
    const auto error = std::from_chars(begin, end, number).second;
    if (error) {
      throw std::runtime_error(error.message());
    }
    return number;
    */
    char *numEnd;
    const long num = std::strtol(begin, &numEnd, 0);
    if (errno == ERANGE || num < std::numeric_limits<int>::lowest() || num > std::numeric_limits<int>::max()) {
      throw std::runtime_error("Number out of range");
    }
    if (num == 0 && numEnd == begin) {
      throw std::runtime_error("Invalid number");
    }
    return static_cast<int>(num);
  }
  
  constexpr int NO_FRAME = std::numeric_limits<int>::max();

  struct PathInfo {
    std::string_view pathWithoutFrame;
    int frame;
  };
  
  PathInfo getInfo(const std::string_view path) {
    size_t frameBegin = path.size();
    size_t frameEnd = frameBegin;
    bool gotFrameNum = false;
    bool passedDot = false;
    for (size_t i = path.size() - 1; i != -size_t(1); --i) {
      const char c = path[i];
      if (passedDot) {
        if (std::isdigit(c)) {
          if (gotFrameNum) {
            frameBegin = i;
          } else {
            frameBegin = i;
            frameEnd = i + 1;
            gotFrameNum = true;
          }
        } else {
          break;
        }
      } else {
        if (c == '.') {
          passedDot = true;
        }
      }
    }
    
    int frame;
    if (gotFrameNum) {
      frame = parseInt(path.data() + frameBegin, path.data() + frameEnd);
    } else {
      frame = NO_FRAME;
    }
    return {path.substr(0, frameBegin), frame};
  }

  bool less(const std::string &left, const std::string &right) {
    const PathInfo leftInfo = getInfo(left);
    const PathInfo rightInfo = getInfo(right);
    if (leftInfo.pathWithoutFrame == rightInfo.pathWithoutFrame) {
      return leftInfo.frame < rightInfo.frame;
    } else {
      return leftInfo.pathWithoutFrame < rightInfo.pathWithoutFrame;
    }
  }
}

void sortByFrame(std::vector<std::string> &paths) {
  std::sort(paths.begin(), paths.end(), less);
}
