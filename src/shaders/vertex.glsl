#version 330 core
layout (location = 0) in uvec4 a_pos;

uniform float u_scale;
uniform ivec2 u_offset;

void main() {
        gl_Position = vec4((ivec2(a_pos.xy) + u_offset) * u_scale - vec2(1.0), 1.0, 1.0);
}
