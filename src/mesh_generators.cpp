#include "mesh_generators.h"

#include <glm/gtc/constants.hpp>
#include <cmath>

/**
 * @brief Generate a unit sphere using recursive subdivision.
 */
mesh_data generate_sphere(unsigned int tesselation_level) {
    mesh_data result;

    auto& vertices  = result.vertices;
    auto& normals   = result.normals;
    auto& indices   = result.indices;

    // Initial octahedron
    vertices = {
        { 1.f,  0.f,  0.f},
        {-1.f,  0.f,  0.f},
        { 0.f,  1.f,  0.f},
        { 0.f, -1.f,  0.f},
        { 0.f,  0.f,  1.f},
        { 0.f,  0.f, -1.f}
    };

    indices = {
        0, 3, 5,   3, 1, 5,
        3, 4, 1,   0, 4, 3,
        2, 0, 5,   2, 5, 1,
        4, 0, 2,   4, 2, 1
    };

    // Subdivide
    for (unsigned int level = 0; level < tesselation_level; ++level) {
        std::vector<unsigned int> new_indices;

        for (size_t i = 0; i < indices.size(); i += 3) {
            const glm::vec3& v0 = vertices[indices[i]];
            const glm::vec3& v1 = vertices[indices[i + 1]];
            const glm::vec3& v2 = vertices[indices[i + 2]];

            glm::vec3 a = glm::normalize((v0 + v1) * 0.5f);
            glm::vec3 b = glm::normalize((v0 + v2) * 0.5f);
            glm::vec3 c = glm::normalize((v1 + v2) * 0.5f);

            unsigned int ia = vertices.size(); vertices.push_back(a);
            unsigned int ib = vertices.size(); vertices.push_back(b);
            unsigned int ic = vertices.size(); vertices.push_back(c);

            new_indices.insert(new_indices.end(), {
                indices[i], ia, ib,
                indices[i + 1], ic, ia,
                indices[i + 2], ib, ic,
                ia, ic, ib
            });
        }

        indices = std::move(new_indices);
    }

    // Normals equal vertices for unit sphere
    normals = vertices;

    return result;
}

/**
 * @brief Generate a unit cylinder mesh (radius 1, height 1).
 */
mesh_data generate_cylinder(unsigned int stack_count,
                            unsigned int slice_count)
{
    mesh_data result;

    auto& vertices = result.vertices;
    auto& normals  = result.normals;
    auto& indices  = result.indices;

    // --------------------
    // Generate vertices and normals
    // --------------------

    for (unsigned int stack = 0; stack < stack_count; ++stack) {
        float z = static_cast<float>(stack) / (stack_count - 1);

        for (unsigned int slice = 0; slice < slice_count; ++slice) {
            float angle = 2.0f * glm::pi<float>() *
                          static_cast<float>(slice) / slice_count;

            float x = std::cos(angle);
            float y = std::sin(angle);

            vertices.emplace_back(x, y, z);
            normals.emplace_back(glm::normalize(glm::vec3(x, y, 0.0f)));
        }
    }

    // --------------------
    // Generate indices
    // --------------------

    for (unsigned int stack = 0; stack < stack_count - 1; ++stack) {
        for (unsigned int slice = 0; slice < slice_count; ++slice) {
            unsigned int next_slice =
                (slice + 1) % slice_count;

            unsigned int a = stack * slice_count + slice;
            unsigned int b = (stack + 1) * slice_count + slice;
            unsigned int c = (stack + 1) * slice_count + next_slice;
            unsigned int d = stack * slice_count + next_slice;

            // Triangle 1
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);

            // Triangle 2
            indices.push_back(a);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    return result;
}