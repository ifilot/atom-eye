#pragma once

#include "mesh_data.h"

/**
 * @brief GPU-resident mesh.
 */
class Mesh {
public:
    explicit Mesh(const MeshData& data);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void draw() const;

private:
    unsigned int vao_;
    unsigned int vbo_[2];
    unsigned int ebo_;
    unsigned int index_count_;
};
