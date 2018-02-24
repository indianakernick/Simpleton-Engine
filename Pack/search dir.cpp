//
//  search dir.cpp
//  Pack
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "search dir.hpp"

#include <dirent.h>
#include <iostream>

DirSearchError::DirSearchError()
  : std::runtime_error("Failed to search for files in this directory") {}

NoSupportedFilesError::NoSupportedFilesError()
  : std::runtime_error("There are no supported files in this directory") {}

bool extIsImage(const std::string_view ext) {
  static const std::string_view EXTS[] = {
    "jpg", "jpeg", "png", "bmp", "psd", "tga", "gif", "hdr", "pic", "pgm", "ppm"
  };
  
  return std::any_of(std::cbegin(EXTS), std::cend(EXTS), [ext](const std::string_view thisExt) {
    return ext == thisExt;
  });
}

std::string_view getExt(const std::string_view path) {
  const size_t lastDot = path.find_last_of('.');
  if (lastDot >= path.size() - 1) {
    return {"", 0};
  } else {
    return {
      path.data() + (lastDot + 1),
      path.size() - (lastDot + 1)
    };
  }
}

void findFilesHelper(
  const std::string_view path,
  const SearchPred pred,
  const size_t maxDepth,
  std::vector<std::string> &files
) {
  if (maxDepth == 0) {
    return;
  }
  
  std::cout << "Searching directory \"" << path << "\"\n";
  
  std::unique_ptr<DIR, int(*)(DIR *)> dir(opendir(path.data()), closedir);
  if (dir) {
    while (const dirent *entry = readdir(dir.get())) {
      if (entry->d_type == DT_DIR) {
        findFilesHelper(path, pred, maxDepth - 1, files);
      } else if (entry->d_type == DT_REG) {
        const std::string_view ext = getExt(entry->d_name);
        if (ext.size() && pred(ext)) {
          files.emplace_back(std::string(path) + '/' + entry->d_name);
        }
      }
    }
  } else {
    throw DirSearchError();
  }
}

std::vector<std::string> findFiles(
  const std::string_view path,
  const SearchPred pred,
  const size_t maxDepth
) {
  std::vector<std::string> files;
  
  findFilesHelper(path, pred, maxDepth, files);
  
  if (files.size() == 0) {
    throw NoSupportedFilesError();
  }
  
  return files;
}
