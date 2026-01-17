#version 330 core

in vec3 v_normal;
in vec3 v_view_dir;

uniform vec3 base_color;
uniform vec3 light_dir;

uniform float rim_strength;
uniform float rim_power;
uniform int toon_levels;

out vec4 out_color;

void main()
{
    vec3 N = normalize(v_normal);
    vec3 V = normalize(v_view_dir);
    vec3 L = normalize(light_dir);

    // --------------------
    // Toon diffuse
    // --------------------
    float ndotl = max(dot(N, L), 0.0);
    float toon_diffuse = floor(ndotl * float(toon_levels)) / float(toon_levels);

    // Ambient floor (KEY FIX)
    float ambient = 0.25;
    float lighting = ambient + (1.0 - ambient) * toon_diffuse;

    // --------------------
    // Rim shading
    // --------------------
    float ndotv = clamp(dot(N, V), 0.0, 1.0);
    float rim = pow(1.0 - ndotv, rim_power);

    // --------------------
    // Combine
    // --------------------
    vec3 color = base_color * lighting;

    // Rim as interpolation
    color = mix(color, vec3(0.0), rim * rim_strength);

    out_color = vec4(color, 1.0);
}
