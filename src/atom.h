#pragma once

#include <glm/glm.hpp>

/**
 * @brief Single atom in a molecule.
 */
struct Atom {
    glm::vec3 position;
    float radius;
    glm::vec3 color;
};