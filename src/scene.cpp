#include "scene.h"
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() = default;

void Scene::add_molecule(Molecule&& molecule)
{
    molecules_.emplace_back(std::move(molecule));
}

void Scene::draw(Shader& shader,
                 const glm::mat4& view,
                 const glm::mat4& proj) const
{
    for (const auto& molecule : molecules_) {
        molecule.draw(shader,
                      view,
                      proj,
                      rotation_x,
                      rotation_y,
                      translation);
    }
}

float Scene::bounding_radius() const
{
    float r = 0.0f;

    for (const auto& molecule : molecules_) {
        r = std::max(r, molecule.bounding_radius());
    }

    return r;
}