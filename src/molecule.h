#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "mesh_library.h"
#include "shader.h"

/**
 * @brief Represents a molecule containing atoms and bonds and can render itself.
 *
 * Supports manual construction via add_atom/add_bond and loading from .xyz files.
 */
class Molecule {
public:
    /**
     * @brief Construct a molecule that uses shared meshes from a mesh library.
     *
     * @param meshes Reference to the shared mesh cache.
     */
    explicit Molecule(MeshLibrary& meshes);

    /**
     * @brief Add a single atom to the molecule (manual construction).
     *
     * @param position Atom position in atomic units.
     * @param radius   Visual radius in atomic units.
     * @param color    Atom RGB color (0..1).
     */
    void add_atom(const glm::vec3& position,
                  float radius,
                  const glm::vec3& color);

    /**
     * @brief Add a single bond to the molecule (manual construction).
     *
     * @param atom_a Index of the first atom.
     * @param atom_b Index of the second atom.
     * @param radius Visual bond radius in atomic units.
     * @param color  Bond RGB color (0..1).
     */
    void add_bond(unsigned int atom_a,
                  unsigned int atom_b,
                  float radius,
                  const glm::vec3& color);

    /**
     * @brief Remove all atoms and bonds.
     */
    void clear();

    /**
     * @brief Load atoms from an XYZ file and (optionally) generate bonds.
     *
     * XYZ format:
     *  line 1: atom count
     *  line 2: comment
     *  lines:  <ElementSymbol> <x> <y> <z>
     *
     * Supported elements: H, C, N, O.
     *
     * @param path File path to the .xyz file.
     * @param generate_bonds If true, bonds are generated using a distance heuristic.
     */
    void load_xyz(const std::string& path, bool generate_bonds = true);

    /**
     * @brief Render the molecule using the current shader style.
     *
     * Expects the shader to use uniforms:
     *  - model, view, proj
     *  - base_color
     *  - rim_strength, rim_power
     *  - light_dir, toon_levels (if toon enabled)
     *
     * @param shader Shader used to render the molecule.
     * @param view   View matrix.
     * @param proj   Projection matrix.
     * @param time   Time in seconds (used for animation).
     */
    void draw(Shader& shader,
              const glm::mat4& view,
              const glm::mat4& proj,
              float rot_x,
              float rot_y,
              const glm::vec3& offset) const;

    float bounding_radius() const;

private:
    /**
     * @brief Atom data stored in the molecule.
     */
    struct Atom {
        glm::vec3 position;
        float radius;
        glm::vec3 color;
        std::string element; // e.g. "C", "H"
    };

    /**
     * @brief Bond data stored in the molecule.
     */
    struct Bond {
        unsigned int atom_a;
        unsigned int atom_b;
        float radius;
        glm::vec3 color;
    };

    /**
     * @brief Generate bonds using a simple distance heuristic.
     *
     * This is not chemistry-accurate. It is a practical visualization heuristic:
     * if distance(i,j) < (r_i + r_j) * factor, a bond is created.
     *
     * @param factor Multiplier for bond cutoff.
     * @param bond_radius Radius to use for all generated bonds.
     * @param bond_color Color to use for all generated bonds.
     */
    void generate_bonds(float factor,
                        float bond_radius,
                        const glm::vec3& bond_color);

    MeshLibrary& meshes_;
    std::vector<Atom> atoms_;
    std::vector<Bond> bonds_;
};
