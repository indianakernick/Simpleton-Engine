//
//  json.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 29/12/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_data_json_hpp
#define engine_data_json_hpp

#include <json.hpp>
#include <glm/vec2.hpp>

using nlohmann::json;

namespace Data {
  template <typename T>
  void getOptional(T &dst, const json &node, const char *name) {
    const auto iter = node.find(name);
    if (iter != node.cend()) {
      dst = iter->get<T>();
    }
  }
}

#define JSON_OPTIONAL(CHILD_ITER, PARENT_NODE, CHILD_NAME)                      \
  const auto CHILD_ITER = PARENT_NODE.find(CHILD_NAME);                         \
  CHILD_ITER != PARENT_NODE.end()

namespace glm {
  inline void from_json(const json &j, glm::vec2 &vec) {
    vec.x = j.at(0).get<float>();
    vec.y = j.at(1).get<float>();
  }
}

#endif
