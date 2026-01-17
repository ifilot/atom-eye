#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.h"

/**
 * @brief Screen-space blueprint-style background renderer.
 *
 * Draws a procedural grid using a fullscreen quad.
 * Intended to be rendered before all 3D geometry.
 */
class Background {
public:
    /**
     * @brief Construct background renderer.
     *
     * @param shader Blueprint background shader.
     */
    explicit Background(Shader& shader);

    /**
     * @brief Destroy background renderer and free GPU resources.
     */
    ~Background();

    /**
     * @brief Draw the background.
     *
     * @param width  Framebuffer width in pixels.
     * @param height Framebuffer height in pixels.
     */
    void draw(int width, int height) const;

private:
    void init_buffers();

    Shader& shader_;

    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLuint ebo_ = 0;
};
