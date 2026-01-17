#include "mesh.h"

#include <GL/glew.h>

Mesh::Mesh(const MeshData& data)
    : vao_(0),
      vbo_{0, 0},
      ebo_(0),
      index_count_(static_cast<unsigned int>(data.indices.size()))
{
    // --------------------
    // Create VAO
    // --------------------
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // --------------------
    // Vertices VBO
    // --------------------
    glGenBuffers(1, &vbo_[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 data.vertices.size() * sizeof(glm::vec3),
                 data.vertices.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // location
        3,                  // vec3
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec3),
        nullptr
    );

    // --------------------
    // Normals VBO
    // --------------------
    glGenBuffers(1, &vbo_[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 data.normals.size() * sizeof(glm::vec3),
                 data.normals.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                  // location
        3,                  // vec3
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec3),
        nullptr
    );

    // --------------------
    // Element buffer
    // --------------------
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 data.indices.size() * sizeof(unsigned int),
                 data.indices.data(),
                 GL_STATIC_DRAW);

    // --------------------
    // Cleanup
    // --------------------
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    if (ebo_) {
        glDeleteBuffers(1, &ebo_);
    }
    glDeleteBuffers(2, vbo_);
    if (vao_) {
        glDeleteVertexArrays(1, &vao_);
    }
}

void Mesh::draw() const
{
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES,
                   index_count_,
                   GL_UNSIGNED_INT,
                   nullptr);
    glBindVertexArray(0);
}
