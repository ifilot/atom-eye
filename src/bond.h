#pragma once

#include <glm/glm.hpp>

/**
 * @brief Represents a bond between two atoms.
 */
struct Bond {
    unsigned int atom_a;
    unsigned int atom_b;
    float radius;
    glm::vec3 color;
};