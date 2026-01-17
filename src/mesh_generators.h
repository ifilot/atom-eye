#pragma once

#include "mesh_data.h"

/**
 * @brief Generate a unit sphere mesh using recursive subdivision.
 *
 * @param tesselation_level Number of subdivision iterations
 * @return Generated mesh data
 */
mesh_data generate_sphere(unsigned int tesselation_level);

/**
 * @brief Generate a unit cylinder mesh (radius 1, height 1).
 *
 * @param stack_count Number of vertical subdivisions
 * @param slice_count Number of radial subdivisions
 * @return Generated mesh data
 */
mesh_data generate_cylinder(unsigned int stack_count,
                            unsigned int slice_count);
