#include "molecule.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "element.h"

#include <fstream>
#include <limits>
#include <stdexcept>
#include <iostream>

/**
 * @brief Build a transform for a cylinder aligned along the bond from a->b.
 *
 * Assumes the cylinder mesh is a unit cylinder centered at origin, aligned with +Z,
 * with height 1 spanning [-0.5, 0.5] in z.
 */
static glm::mat4 bond_transform(const glm::vec3& a,
                               const glm::vec3& b,
                               float radius)
{
    glm::vec3 dir = b - a;
    float length = glm::length(dir);
    if (length < 1e-6f) {
        return glm::mat4(1.0f);
    }

    glm::vec3 dir_n = dir / length;
    glm::vec3 z_axis(0.0f, 0.0f, 1.0f);

    float cos_angle = glm::dot(z_axis, dir_n);
    cos_angle = glm::clamp(cos_angle, -1.0f, 1.0f);

    float angle = std::acos(cos_angle);
    glm::vec3 axis = glm::cross(z_axis, dir_n);

    glm::mat4 model(1.0f);

    // Move to midpoint
    model = glm::translate(model, (a + b) * 0.5f);

    // Rotate +Z onto direction
    if (glm::length(axis) > 1e-6f) {
        model = glm::rotate(model, angle, glm::normalize(axis));
    }

    // Scale radius and length
    model = glm::scale(model, glm::vec3(radius, radius, length));

    return model;
}

Molecule::Molecule(MeshLibrary& meshes)
    : meshes_(meshes)
{
}

void Molecule::add_atom(const glm::vec3& position,
                        float radius,
                        const glm::vec3& color)
{
    atoms_.push_back(Atom{
        position,
        radius,
        color,
        "" // unknown element for manual atoms
    });
}

void Molecule::add_bond(unsigned int atom_a,
                        unsigned int atom_b,
                        float radius,
                        const glm::vec3& color)
{
    bonds_.push_back(Bond{
        atom_a,
        atom_b,
        radius,
        color
    });
}

void Molecule::clear()
{
    atoms_.clear();
    bonds_.clear();
}

void Molecule::load_xyz(const std::string& path, bool do_generate_bonds)
{
    clear();

    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open xyz file: " + path);
    }

    std::size_t atom_count = 0;
    file >> atom_count;

    // Skip remainder of first line and the comment line.
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (std::size_t i = 0; i < atom_count; ++i) {
        std::string symbol;
        float x = 0.f, y = 0.f, z = 0.f;

        file >> symbol >> x >> y >> z;
        if (!file) {
            throw std::runtime_error("Malformed xyz file (atom line parse failed): " + path);
        }

        const ElementStyle& style = element_style(symbol);

        atoms_.push_back(Atom{
            glm::vec3(x, y, z),
            style.radius,
            style.color,
            symbol
        });
    }

    if (do_generate_bonds) {
        generate_bonds(1.35f, 0.15f, glm::vec3(0.80f));
    }
}

void Molecule::generate_bonds(float factor,
                              float bond_radius,
                              const glm::vec3& bond_color)
{
    bonds_.clear();

    for (std::size_t i = 0; i < atoms_.size(); ++i) {
        for (std::size_t j = i + 1; j < atoms_.size(); ++j) {
            float d = glm::length(atoms_[i].position - atoms_[j].position);
            float cutoff = (atoms_[i].radius + atoms_[j].radius) * factor;

            if (d < cutoff) {
                bonds_.push_back(Bond{
                    static_cast<unsigned int>(i),
                    static_cast<unsigned int>(j),
                    bond_radius,
                    bond_color
                });
            }
        }
    }
}

void Molecule::draw(Shader& shader,
                    const glm::mat4& view,
                    const glm::mat4& proj,
                    float rot_x,
                    float rot_y,
                    const glm::vec3& offset) const 
{
    const Mesh& sphere   = meshes_.sphere(4);
    const Mesh& cylinder = meshes_.cylinder(16, 16);

    shader.bind();
    shader.set_vec3("light_dir", glm::value_ptr(glm::normalize(glm::vec3(0.3f, 0.7f, 1.0f))));
    
    shader.set_float("specular_strength", 1.0);
    shader.set_float("shininess", 32.0);

    // Per-frame uniforms for view-dependent shading
    shader.set_mat4("view", glm::value_ptr(view));
    shader.set_mat4("proj", glm::value_ptr(proj));

    // rim effects
    shader.set_float("rim_strength", 1.6f);
    shader.set_float("rim_power", 2.5f);

    glm::mat4 molecule_model(1.0f);
    // Translation FIRST (world space)
    molecule_model = glm::translate(molecule_model, offset);

    // Then rotation
    molecule_model = glm::rotate(molecule_model, rot_x, glm::vec3(1.f, 0.f, 0.f));
    molecule_model = glm::rotate(molecule_model, rot_y, glm::vec3(0.f, 1.f, 0.f));

    // ---- Atoms ----
    for (const auto& atom : atoms_) {
        glm::mat4 model =
            molecule_model *
            glm::translate(glm::mat4(1.0f), atom.position) *
            glm::scale(glm::mat4(1.0f), glm::vec3(atom.radius));

        shader.set_mat4("model", glm::value_ptr(model));
        shader.set_vec3("base_color", glm::value_ptr(atom.color));

        sphere.draw();
    }

    // ---- Bonds ----
    for (const auto& bond : bonds_) {
        const glm::vec3& a = atoms_[bond.atom_a].position;
        const glm::vec3& b = atoms_[bond.atom_b].position;

        glm::mat4 model = molecule_model * bond_transform(a, b, bond.radius);

        shader.set_mat4("model", glm::value_ptr(model));
        shader.set_vec3("base_color", glm::value_ptr(bond.color));

        cylinder.draw();
    }
}

float Molecule::bounding_radius() const {
    float r = 0.0f;

    for (const auto& atom : atoms_) {
        float d = glm::length(atom.position) + atom.radius;
        r = std::max(r, d);
    }

    return r;
}
