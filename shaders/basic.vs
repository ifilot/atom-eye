#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;

out vec3 frag_color;

uniform mat4 mvp;

void main()
{
    frag_color = normalize(in_normal) * 0.5 + 0.5;
    gl_Position = mvp * vec4(in_pos, 1.0);
}