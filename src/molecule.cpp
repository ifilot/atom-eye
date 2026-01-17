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
    const Mesh& sphere = meshes_.sphere(4);
    const Mesh& cylinder = meshes_.cylinder(16, 16);

    shader.bind();

    glm::mat4 molecule_model(1.0f);
    molecule_model = glm::rotate(molecule_model, time, glm::vec3(0.f, 1.f, 0.f));

    // ====================
    // Outline pass
    // ====================
    glCullFace(GL_FRONT);
    glDepthMask(GL_FALSE);

    shader.bind();

    // ====================
    // Normal pass
    // ====================
    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);

    shader.set_float("outline_thickness", 0.0f);

    // ---- Atoms ----
    for (const auto& atom : atoms_) {
        glm::mat4 model =
            molecule_model *
            glm::translate(glm::mat4(1.0f), atom.position) *
            glm::scale(glm::mat4(1.0f), glm::vec3(atom.radius));

        glm::mat4 mvp = proj * view * model;

        shader.set_mat4("mvp", glm::value_ptr(mvp));
        shader.set_vec3("color", glm::value_ptr(atom.color));

        meshes_.sphere(3).draw();
    }

    // ---- Bonds ----
    for (const auto& bond : bonds_) {
        const glm::vec3& a = atoms_[bond.atom_a].position;
        const glm::vec3& b = atoms_[bond.atom_b].position;

        glm::mat4 model =
            molecule_model *
            bond_transform(a, b, bond.radius);

        glm::mat4 mvp = proj * view * model;

        shader.set_mat4("mvp", glm::value_ptr(mvp));
        shader.set_vec3("color", glm::value_ptr(bond.color));

        meshes_.cylinder(16, 16).draw();
    }
}
