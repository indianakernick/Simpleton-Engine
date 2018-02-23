//
//  compose string.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 22/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

inline Utils::ComposeString::ComposeString(const size_t size) {
  string.reserve(size);
}

inline std::string_view Utils::ComposeString::view() const {
  return string;
}

inline bool Utils::ComposeString::empty() const {
  return string.empty();
}

inline void Utils::ComposeString::write(const char *const str) {
  string.append(str);
}

inline void Utils::ComposeString::write(const char *const str, const size_t size) {
  string.append(str, size);
}

inline void Utils::ComposeString::write(const std::string_view view) {
  string.append(view.data(), view.size());
}

template <typename Number>
void Utils::ComposeString::writeNumber(const Number number) {
  string += std::to_string(number);
}

template <typename Enum>
void Utils::ComposeString::writeEnum(const Enum e, const std::string_view *names) {
  write(names[static_cast<size_t>(e)]);
}

inline void Utils::ComposeString::open(const std::string &opening, const std::string &closing) {
  write(opening);
  closingStrings.push(closing);
}

inline void Utils::ComposeString::close() {
  write(closingStrings.top());
  closingStrings.pop();
}

inline void Utils::ComposeString::closeAll() {
  while (!closingStrings.empty()) {
    close();
  }
}
