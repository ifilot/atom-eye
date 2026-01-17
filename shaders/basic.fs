#version 330 core

in vec3 v_normal;
in vec3 v_view_dir;

uniform vec3 base_color;
uniform vec3 light_dir;

// Phong controls
uniform float specular_strength;  // e.g. 0.5
uniform float shininess;          // e.g. 32.0

// Rim lighting
uniform float rim_strength;
uniform float rim_power;

out vec4 out_color;

void main()
{
    vec3 N = normalize(v_normal);
    vec3 V = normalize(v_view_dir);
    vec3 L = normalize(light_dir);

    // --------------------
    // Smooth diffuse (Lambert)
    // --------------------
    float ndotl = max(dot(N, L), 0.0);

    float ambient = 0.40;
    float lighting = ambient + (1.0 - ambient) * ndotl;

    // --------------------
    // Phong specular
    // --------------------
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(R, V), 0.0), shininess);
    vec3 specular = specular_strength * spec * vec3(1.0);

    // --------------------
    // Rim lighting
    // --------------------
    float ndotv = clamp(dot(N, V), 0.0, 1.0);
    float rim = pow(1.0 - ndotv, rim_power);

    // --------------------
    // Combine
    // --------------------
    vec3 color = base_color * lighting;
    color += specular;

    // Dark rim (as before)
    color = mix(color, vec3(0.0), rim * rim_strength);

    out_color = vec4(color, 1.0);
}
