#include "background.h"

#include <glm/gtc/type_ptr.hpp>

/**
 * Fullscreen quad vertices (NDC).
 */
static constexpr float quad_vertices[] = {
    -1.f, -1.f,
     1.f, -1.f,
     1.f,  1.f,
    -1.f,  1.f
};

/**
 * Indices for two triangles.
 */
static constexpr unsigned int quad_indices[] = {
    0, 1, 2,
    2, 3, 0
};

Background::Background(Shader& shader)
    : shader_(shader)
{
    init_buffers();
}

Background::~Background()
{
    glDeleteBuffers(1, &ebo_);
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void Background::init_buffers()
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(quad_vertices),
                 quad_vertices,
                 GL_STATIC_DRAW);

    // layout(location = 0) in vec2 in_pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        2 * sizeof(float),
        nullptr
    );

    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(quad_indices),
                 quad_indices,
                 GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Background::draw(int width, int height) const
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shader_.bind();
    shader_.set_vec2("resolution", glm::value_ptr(glm::vec2(
        static_cast<float>(width),
        static_cast<float>(height)))
    );

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}
