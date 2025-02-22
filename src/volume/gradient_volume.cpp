#include "gradient_volume.h"
#include <algorithm>
#include <exception>
#include <glm/geometric.hpp>
#include <glm/vector_relational.hpp>
#include <gsl/span>

namespace volume {

// Compute the maximum magnitude from all gradient voxels
static float computeMaxMagnitude(gsl::span<const GradientVoxel> data)
{
    return std::max_element(
        std::begin(data),
        std::end(data),
        [](const GradientVoxel& lhs, const GradientVoxel& rhs) {
            return lhs.magnitude < rhs.magnitude;
        })
        ->magnitude;
}

// Compute the minimum magnitude from all gradient voxels
static float computeMinMagnitude(gsl::span<const GradientVoxel> data)
{
    return std::min_element(
        std::begin(data),
        std::end(data),
        [](const GradientVoxel& lhs, const GradientVoxel& rhs) {
            return lhs.magnitude < rhs.magnitude;
        })
        ->magnitude;
}

// Compute a gradient volume from a volume
static std::vector<GradientVoxel> computeGradientVolume(const Volume& volume)
{
    const auto dim = volume.dims();

    std::vector<GradientVoxel> out(static_cast<size_t>(dim.x * dim.y * dim.z));
    for (int z = 1; z < dim.z - 1; z++) {
        for (int y = 1; y < dim.y - 1; y++) {
            for (int x = 1; x < dim.x - 1; x++) {
                const float gx = (volume.getVoxel(x + 1, y, z) - volume.getVoxel(x - 1, y, z)) / 2.0f;
                const float gy = (volume.getVoxel(x, y + 1, z) - volume.getVoxel(x, y - 1, z)) / 2.0f;
                const float gz = (volume.getVoxel(x, y, z + 1) - volume.getVoxel(x, y, z - 1)) / 2.0f;

                const glm::vec3 v { gx, gy, gz };
                const size_t index = static_cast<size_t>(x + dim.x * (y + dim.y * z));
                out[index] = GradientVoxel { v, glm::length(v) };
            }
        }
    }
    return out;
}

GradientVolume::GradientVolume(const Volume& volume)
    : m_dim(volume.dims())
    , m_data(computeGradientVolume(volume))
    , m_minMagnitude(computeMinMagnitude(m_data))
    , m_maxMagnitude(computeMaxMagnitude(m_data))
{
}

float GradientVolume::maxMagnitude() const
{
    return m_maxMagnitude;
}

float GradientVolume::minMagnitude() const
{
    return m_minMagnitude;
}

glm::ivec3 GradientVolume::dims() const
{
    return m_dim;
}

// This function returns a gradientVoxel at coord based on the current interpolation mode.
GradientVoxel GradientVolume::getGradientInterpolate(const glm::vec3& coord) const
{
    switch (interpolationMode) {
    case InterpolationMode::NearestNeighbour: {
        return getGradientNearestNeighbor(coord);
    }
    case InterpolationMode::Linear: {
        return getGradientLinearInterpolate(coord);
    }
    case InterpolationMode::Cubic: {
        // No cubic in this case, linear is good enough for the gradient.
        return getGradientLinearInterpolate(coord);
    }
    default: {
        throw std::exception();
    }
    };
}

// This function returns the nearest neighbour given a position in the volume given by coord.
// Notice that in this framework we assume that the distance between neighbouring voxels is 1 in all directions
GradientVoxel GradientVolume::getGradientNearestNeighbor(const glm::vec3& coord) const
{
    if (glm::any(glm::lessThan(coord, glm::vec3(0))) || glm::any(glm::greaterThanEqual(coord, glm::vec3(m_dim))))
        return { glm::vec3(0.0f), 0.0f };

    auto roundToPositiveInt = [](float f) {
        return static_cast<int>(f + 0.5f);
    };

    return getGradient(roundToPositiveInt(coord.x), roundToPositiveInt(coord.y), roundToPositiveInt(coord.z));
}

// ======= TODO : IMPLEMENT ========
// Returns the trilinearly interpolated gradinet at the given coordinate.
// Use the linearInterpolate function that you implemented below.
GradientVoxel GradientVolume::getGradientLinearInterpolate(const glm::vec3& coord) const
{

//     double x0 =  glm::floor(coord[0]);
//     double x1 =  glm::ceil(coord[0]);


//     double y0 =  glm::floor(coord[1]);
//     double y1 =  glm::ceil(coord[1]);
    
    
//     double z0 =  glm::floor(coord[2]);
//     double z1 =   glm::ceil(coord[2]);
    
//     // Compute the distance from the floor of the coordinate (d1) to the coordinate in all dimensions
//     // The distance between the surrounding voxels (d2) for each dimension is 1.
//     // So it is equal to the ratio between d1 and d2,
//     float x_ratio = (float)(coord[0] - x0);
//     float y_ratio = (float)(coord[1] - y0);
//     float z_ratio = (float)(coord[2] - z0);
    


//     // Bilineaire Interpolatie over y0 = floor y 
//     GradientVoxel c00 = linearInterpolate(getGradient((int) x0, (int) y0, (int) z0),
//                                     getGradient((int) x1, (int) y0, (int) z0),
//                                     x_ratio);                                        
//     GradientVoxel c01 = linearInterpolate(getGradient((int) x0, (int) y0, (int) z1),
//                                     getGradient((int) x1, (int) y0, (int) z1),
//                                     x_ratio);




//     GradientVoxel c10 = linearInterpolate(getGradient((int) x0, (int) y1, (int) z0),
//                                     getGradient((int) x1, (int) y1, (int) z0),
//                                     x_ratio);
//     GradientVoxel c11 = linearInterpolate(getGradient((int) x0, (int) y1, (int) z1),
//                                     getGradient((int) x1, (int) y1, (int) z1),
//                                     x_ratio);
//     GradientVoxel c0 = linearInterpolate(c00, c10, y_ratio);
//     GradientVoxel c1 = linearInterpolate(c01,c11, y_ratio);
    
//     GradientVoxel gv = linearInterpolate(c0, c1, z_ratio);
//     return GradientVoxel {gv};
    return GradientVoxel {};
}

// ======= TODO : IMPLEMENT ========
// This function should linearly interpolates the value from g0 to g1 given the factor (t).
// At t=0, linearInterpolate should return g0 and at t=1 it returns g1.
GradientVoxel GradientVolume::linearInterpolate(const GradientVoxel& g0, const GradientVoxel& g1, float factor)
{
    return GradientVoxel {};
    // return GradientVoxel {(1 - factor) * g0.x + factor * g1.x, (1 - factor) * g0.y + factor * g1.y, (1 - factor) * g0.z + factor * g1.z};
}



// This function returns a gradientVoxel without using interpolation
GradientVoxel GradientVolume::getGradient(int x, int y, int z) const
{
    const size_t i = static_cast<size_t>(x + m_dim.x * (y + m_dim.y * z));
    return m_data[i];
}
}