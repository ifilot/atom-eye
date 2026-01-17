#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 v_normal;
out vec3 v_view_dir;

void main()
{
    vec4 world_pos = model * vec4(in_pos, 1.0);

    mat3 normal_matrix = transpose(inverse(mat3(model)));
    v_normal = normalize(normal_matrix * in_normal);

    vec3 camera_pos = vec3(inverse(view)[3]);
    v_view_dir = normalize(camera_pos - world_pos.xyz);

    gl_Position = proj * view * world_pos;
}
