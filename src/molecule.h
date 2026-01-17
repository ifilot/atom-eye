#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "atom.h"
#include "mesh_library.h"
#include "shader.h"

/**
 * @brief Renderable molecule composed of atoms.
 */
class Molecule {
public:
    explicit Molecule(MeshLibrary& mesh_library);

    void add_atom(const Atom& atom);

    void draw(Shader& shader,
              const glm::mat4& view,
              const glm::mat4& proj,
              float time) const;

private:
    MeshLibrary& meshes_;
    std::vector<Atom> atoms_;
};
