#include "molecule.h"

#include <glm/gtc/matrix_transform.hpp>

Molecule::Molecule(MeshLibrary& mesh_library)
    : meshes_(mesh_library)
{
}

void Molecule::add_atom(const Atom& atom)
{
    atoms_.push_back(atom);
}

void Molecule::draw(Shader& shader,
                    const glm::mat4& view,
                    const glm::mat4& proj,
                    float time) const
{
    const Mesh& sphere = meshes_.sphere(3);

    shader.bind();

    for (const auto& atom : atoms_) {
        glm::mat4 model(1.0f);

        model = glm::rotate(model, time, glm::vec3(0.f, 1.f, 0.f));
        model = glm::translate(model, atom.position);
        model = glm::scale(model, glm::vec3(atom.radius));

        glm::mat4 mvp = proj * view * model;

        shader.set_mat4("mvp", &mvp[0][0]);
        shader.set_vec3("color", &atom.color[0]);

        sphere.draw();
    }
}
