#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "atom.h"
#include "bond.h"
#include "mesh_library.h"
#include "shader.h"

/**
 * @brief Renderable molecule composed of atoms.
 */
class Molecule {
public:
    explicit Molecule(MeshLibrary& mesh_library);

    void add_atom(const Atom& atom);

    void add_bond(unsigned int atom_a,
                  unsigned int atom_b,
                  float radius,
                  const glm::vec3& color);

    void draw(Shader& shader,
              const glm::mat4& view,
              const glm::mat4& proj,
              float time) const;

private:
    MeshLibrary& meshes_;
    std::vector<Atom> atoms_;
    std::vector<Bond> bonds_;
};
