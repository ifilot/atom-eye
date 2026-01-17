#pragma once

#include <glm/glm.hpp>
#include <string>

/**
 * @brief Visual style for an element (atomic units for now).
 */
struct ElementStyle {
    float radius;
    glm::vec3 color;
};

/**
 * @brief Lookup element style by symbol (H,C,N,O supported).
 *
 * @throws std::runtime_error if symbol is unsupported.
 */
const ElementStyle& element_style(const std::string& symbol);
