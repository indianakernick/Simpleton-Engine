//
//  shaders.hpp
//  Simpleton Engine
//
//  Created by Indi Kernick on 11/2/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#ifndef engine_graphics_2d_shaders_hpp
#define engine_graphics_2d_shaders_hpp

namespace G2D {
  const char DESKTOP_SHADER_VERSION[] = "#version 330 core\n";
  const char ES_SHADER_VERSION[] = "#version 300 es\nprecision mediump float;\n";
  
  const char VERT_SHADER[] = R"delimiter(
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;

out vec2 fragTexCoord;

uniform mat3 viewProj;

void main() {
  gl_Position.xy = (viewProj * vec3(pos.xy, 1.0)).xy;
  gl_Position.zw = vec2(pos.z, 1.0);
  fragTexCoord = texCoord;
}
)delimiter";

  const char FRAG_SHADER[] = R"delimiter(
in vec2 fragTexCoord;

uniform sampler2D tex;
uniform vec4 color;

out vec4 outColor;

void main() {
  outColor = color * texture(tex, fragTexCoord);
  gl_FragDepth = (outColor.a == 0.0 ? 1.0 : gl_FragCoord.z);
}
)delimiter";
}

#endif
