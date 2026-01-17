#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;

uniform mat4 mvp;

void main()
{
    vec3 pos = in_pos;
    gl_Position = mvp * vec4(pos, 1.0);
}