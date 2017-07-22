//
//  file io.cpp
//  Game Engine
//
//  Created by Indi Kernick on 20/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "file io.hpp"

namespace {
  struct FileCloser {
    void operator()(std::FILE *const file) {
      std::fclose(file);
    }
  };

  using FileHandle = std::unique_ptr<std::FILE, FileCloser>;
  
  FileHandle openFileRead(const char *const path) {
    assert(path);
    std::FILE *const file = std::fopen(path, "rb");
    if (file == nullptr) {
      throw Memory::FileError("Failed to open file for reading");
    } else {
      return {file, {}};
    }
  }
  
  FileHandle openFileWrite(const char *const path) {
    assert(path);
    std::FILE *const file = std::fopen(path, "wb");
    if (file == nullptr) {
      throw Memory::FileError("Failed to open file for writing");
    } else {
      return {file, {}};
    }
  }
}

Memory::FileError::FileError(const char *const what)
  : std::runtime_error(what) {}

Memory::Buffer Memory::readFile(const std::string &path) {
  return readFile(path.c_str());
}

Memory::Buffer Memory::readFile(const char *const path) {
  return readFile(openFileRead(path).get());
}

Memory::Buffer Memory::readFile(std::FILE *const file) {
  std::fseek(file, 0, SEEK_END);
  Memory::Buffer buf(std::ftell(file));
  std::rewind(file);
  if (std::fread(buf.data(), buf.size(), 1, file)) {
    return buf;
  } else {
    throw FileError("Failed to read from file");
  }
}

Memory::Buffer Memory::readFile(std::istream &stream) {
  stream.seekg(0, std::ios_base::seekdir::end);
  Memory::Buffer buf(stream.tellg());
  stream.seekg(0, std::ios_base::seekdir::beg);
  stream.read(buf.data<std::istream::char_type>(), buf.size());
  if (stream.good()) {
    return buf;
  } else {
    throw FileError("Failed to read from input stream");
  }
}

void Memory::writeFile(const Memory::Buffer &buf, const std::string &path) {
  writeFile(buf, path.c_str());
}

void Memory::writeFile(const Memory::Buffer &buf, const char *const path) {
  writeFile(buf, openFileWrite(path).get());
}

void Memory::writeFile(const Memory::Buffer &buf, std::FILE *const file) {
  if (std::fwrite(buf.data(), buf.size(), 1, file) == 0) {
    throw FileError("Failed to write to file");
  }
  std::fflush(file);
}

void Memory::writeFile(const Memory::Buffer &buf, std::ostream &stream) {
  stream.write(buf.data<std::ostream::char_type>(), buf.size<std::streamsize>());
  stream.flush();
  if (!stream.good()) {
    throw FileError("Failed to write to output stream");
  }
}
