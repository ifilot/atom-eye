#pragma once

#include <unordered_map>
#include <memory>

#include "mesh.h"

/**
 * @brief Central cache for shared meshes.
 */
class MeshLibrary {
public:
    const Mesh& sphere(unsigned int tessellation);
    const Mesh& cylinder(unsigned int stacks, unsigned int slices);

private:
    std::unordered_map<unsigned int, std::unique_ptr<Mesh>> spheres_;
    std::unordered_map<uint64_t, std::unique_ptr<Mesh>> cylinders_;

    static uint64_t cylinder_key(unsigned int stacks, unsigned int slices);
};
