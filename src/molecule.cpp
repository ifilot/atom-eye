#include "molecule.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

Molecule::Molecule(MeshLibrary& mesh_library)
    : meshes_(mesh_library)
{
}

void Molecule::add_atom(const Atom& atom)
{
    atoms_.push_back(atom);
}

void Molecule::add_bond(unsigned int atom_a,
                        unsigned int atom_b,
                        float radius,
                        const glm::vec3& color)
{
    bonds_.push_back({
        atom_a,
        atom_b,
        radius,
        color
    });
}

static glm::mat4 bond_transform(const glm::vec3& a,
                                const glm::vec3& b,
                                float radius)
{
    glm::vec3 dir = b - a;
    float length = glm::length(dir);
    glm::vec3 z_axis(0.0f, 0.0f, 1.0f);

    glm::vec3 axis = glm::cross(z_axis, glm::normalize(dir));
    float angle = std::acos(glm::dot(z_axis, glm::normalize(dir)));

    glm::mat4 model(1.0f);

    model = glm::translate(model, (a + b) * 0.5f);

    if (glm::length(axis) > 1e-6f) {
        model = glm::rotate(model, angle, glm::normalize(axis));
    }

    model = glm::scale(model, glm::vec3(radius, radius, length));

    return model;
}

void Molecule::draw(Shader& shader,
                    const glm::mat4& view,
                    const glm::mat4& proj,
                    float time) const
{
    const Mesh& sphere   = meshes_.sphere(4);
    const Mesh& cylinder = meshes_.cylinder(16, 16);

    shader.bind();

    // --------------------
    // Per-frame uniforms
    // --------------------
    shader.set_mat4("view", glm::value_ptr(view));
    shader.set_mat4("proj", glm::value_ptr(proj));

    shader.set_vec3("light_dir", glm::value_ptr(glm::normalize(glm::vec3(0.3f, 0.7f, 1.0f))));
    shader.set_int("toon_levels", 4);

    // Rim shading parameters (tweakable)
    shader.set_float("rim_strength", 1.2f);
    shader.set_float("rim_power", 3.5f);

    // --------------------
    // Molecule transform
    // --------------------
    glm::mat4 molecule_model(1.0f);
    molecule_model = glm::rotate(molecule_model, time, glm::vec3(0.f, 1.f, 0.f));

    // --------------------
    // Draw atoms
    // --------------------
    for (const auto& atom : atoms_) {
        glm::mat4 model =
            molecule_model *
            glm::translate(glm::mat4(1.0f), atom.position) *
            glm::scale(glm::mat4(1.0f), glm::vec3(atom.radius));

        shader.set_mat4("model", glm::value_ptr(model));
        shader.set_vec3("base_color", glm::value_ptr(atom.color));

        sphere.draw();
    }

    // --------------------
    // Draw bonds
    // --------------------
    for (const auto& bond : bonds_) {
        const glm::vec3& a = atoms_[bond.atom_a].position;
        const glm::vec3& b = atoms_[bond.atom_b].position;

        glm::mat4 model =
            molecule_model *
            bond_transform(a, b, bond.radius);

        shader.set_mat4("model", glm::value_ptr(model));
        shader.set_vec3("base_color", glm::value_ptr(bond.color));

        cylinder.draw();
    }
}
