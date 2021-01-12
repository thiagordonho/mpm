#include "particle.h"
#include "factory.h"
#include "particle_base.h"

namespace mpm {
// ParticleType
std::map<std::string, int> ParticleType = {{"P2D", 0}, {"P3D", 1}};
std::map<int, std::string> ParticleTypeName = {{0, "P2D"}, {1, "P3D"}};
}  // namespace mpm

// Particle2D (2 Dim)
static Register<mpm::ParticleBase<2>, mpm::Particle<2>, mpm::Index,
                const Eigen::Matrix<double, 2, 1>&>
    particle2d("P2D");

// Particle3D (3 Dim)
static Register<mpm::ParticleBase<3>, mpm::Particle<3>, mpm::Index,
                const Eigen::Matrix<double, 3, 1>&>
    particle3d("P3D");
