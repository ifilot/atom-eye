#include "mesh.h"

#include <GL/glew.h>

mesh::mesh(const mesh_data& data)
    : index_count_(data.indices.size())
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    struct vertex {
        float px, py, pz;
        float nx, ny, nz;
    };

    std::vector<vertex> packed;
    packed.reserve(data.vertices.size());

    for (size_t i = 0; i < data.vertices.size(); ++i) {
        packed.push_back({
            data.vertices[i].x,
            data.vertices[i].y,
            data.vertices[i].z,
            data.normals[i].x,
            data.normals[i].y,
            data.normals[i].z
        });
    }

    glBufferData(GL_ARRAY_BUFFER,
                 packed.size() * sizeof(vertex),
                 packed.data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 data.indices.size() * sizeof(unsigned int),
                 data.indices.data(),
                 GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertex), (void*)0);

    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertex),
                          (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

mesh::~mesh() {
    if (ebo_) glDeleteBuffers(1, &ebo_);
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
}

mesh::mesh(mesh&& other) noexcept {
    *this = std::move(other);
}

mesh& mesh::operator=(mesh&& other) noexcept {
    vao_ = other.vao_;
    vbo_ = other.vbo_;
    ebo_ = other.ebo_;
    index_count_ = other.index_count_;

    other.vao_ = other.vbo_ = other.ebo_ = 0;
    other.index_count_ = 0;
    return *this;
}

void mesh::draw() const {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, nullptr);
}
