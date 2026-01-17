#pragma once

#include "mesh_data.h"

/**
 * @brief GPU-side mesh representation.
 *
 * Uploads mesh data to OpenGL buffers and manages their lifetime.
 */
class mesh {
public:
    explicit mesh(const mesh_data& data);
    ~mesh();

    mesh(const mesh&) = delete;
    mesh& operator=(const mesh&) = delete;

    mesh(mesh&& other) noexcept;
    mesh& operator=(mesh&& other) noexcept;

    /**
     * @brief Draw the mesh.
     */
    void draw() const;

private:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    unsigned int ebo_ = 0;
    unsigned int index_count_ = 0;
};
