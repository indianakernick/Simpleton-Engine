//
//  rotate to.inl
//  Simpleton Engine
//
//  Created by Indi Kernick on 11/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

inline Cam2D::Rotate::Rotate(const Dir dir, const bool enabled)
  : dir(dir), enabled(enabled) {}

inline void Cam2D::Rotate::setDir(const Dir newDir) {
  dir = newDir;
}

inline void Cam2D::Rotate::setEnabled(const bool newEnabled) {
  enabled = newEnabled;
}

inline float Cam2D::Rotate::calcTarget(const Props props, Params) {
  if (enabled) {
    constexpr float offset = glm::pi<float>() - 0.000001f;
    if (dir == Dir::CCW) {
      return (lastAngle = props.angle + offset);
    } else {
      return (lastAngle = props.angle - offset);
    }
  } else {
    return lastAngle;
  }
}
