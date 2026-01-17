#pragma once

#include <vector>
#include <glm/glm.hpp>

/**
 * @brief CPU-side mesh data container.
 *
 * Holds raw geometry data that can be uploaded to the GPU.
 * This structure has no OpenGL dependencies.
 */
struct MeshData {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
};
