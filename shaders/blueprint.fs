#version 330 core

in vec2 v_uv;
out vec4 out_color;

uniform vec2 resolution;
uniform vec3 base_color;

void main()
{
    // Blueprint base color
    vec3 base = base_color;

    // Grid scale
    float minor_scale = 10.0;
    float major_scale = 10.0;

    // ---- CENTERING FIX ----
    vec2 uv = v_uv - 0.5;

    vec2 grid = uv * resolution / minor_scale;

    vec2 line = abs(fract(grid - 0.5) - 0.5) / fwidth(grid);
    float minor = 1.0 - min(min(line.x, line.y), 1.0);

    vec2 major_grid = uv * resolution / (minor_scale * major_scale);
    vec2 major_line = abs(fract(major_grid - 0.5) - 0.5) / fwidth(major_grid);
    float major = 1.0 - min(min(major_line.x, major_line.y), 1.0);

    vec3 color = base;
    color += minor * vec3(0.10);
    color += major * vec3(0.25);

    out_color = vec4(color, 1.0);
}
