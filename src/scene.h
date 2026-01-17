#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "molecule.h"
#include "shader.h"

class Scene {
public:
    Scene();

    void add_molecule(Molecule&& molecule);

    void draw(Shader& shader,
              const glm::mat4& view,
              const glm::mat4& proj) const;

    // Scene transform (what the user manipulates)
    glm::vec3 translation {0.0f};
    float rotation_x = 0.0f;
    float rotation_y = 0.0f;

    float bounding_radius() const;

private:
    std::vector<Molecule> molecules_;
};
