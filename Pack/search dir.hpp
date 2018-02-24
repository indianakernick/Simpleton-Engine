//
//  search dir.hpp
//  Pack
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef search_dir_hpp
#define search_dir_hpp

#include <vector>
#include <string>
#include <string_view>

class DirSearchError final : public std::runtime_error {
public:
  DirSearchError();
};

class NoSupportedFilesError final : public std::runtime_error {
public:
  NoSupportedFilesError();
};

using SearchPred = bool(*)(std::string_view);

bool extIsImage(std::string_view);
std::vector<std::string> findFiles(std::string_view, SearchPred);
std::vector<std::string> findFilesRec(std::string_view, SearchPred, size_t);

#endif
