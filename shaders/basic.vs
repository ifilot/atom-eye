#version 330 core

// Vertex attributes (must match glVertexAttribPointer calls)
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec3 inColor;

// Passed to fragment shader
out vec3 fragColor;

// Uniforms
uniform mat4 mvp;

void main() {
    fragColor = inColor;
    gl_Position = mvp * vec4(inPos, 0.0, 1.0);
}
