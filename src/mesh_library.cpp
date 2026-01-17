#include "mesh_library.h"
#include "mesh_generators.h"

uint64_t MeshLibrary::cylinder_key(unsigned int stacks,
                                   unsigned int slices)
{
    return (uint64_t(stacks) << 32) | slices;
}

const Mesh& MeshLibrary::sphere(unsigned int tessellation)
{
    auto it = spheres_.find(tessellation);
    if (it != spheres_.end()) {
        return *it->second;
    }

    auto mesh = std::make_unique<Mesh>(
        generate_sphere(tessellation)
    );

    const Mesh& ref = *mesh;
    spheres_[tessellation] = std::move(mesh);
    return ref;
}

const Mesh& MeshLibrary::cylinder(unsigned int stacks,
                                  unsigned int slices)
{
    uint64_t key = cylinder_key(stacks, slices);

    auto it = cylinders_.find(key);
    if (it != cylinders_.end()) {
        return *it->second;
    }

    auto mesh = std::make_unique<Mesh>(generate_cylinder(stacks, slices));

    const Mesh& ref = *mesh;
    cylinders_[key] = std::move(mesh);
    return ref;
}
