#ifndef MPM_STANDARD_POINT_GENERATOR_H_
#define MPM_STANDARD_POINT_GENERATOR_H_


#include "point_generator.h"

namespace mpm {

//! StandardPointGenerator class
//! \brief Base class that defines generation of material points
//! \tparam Tdim Dimension
template <unsigned Tdim>
class StandardPointGenerator : public PointGenerator<Tdim> {
 public:
  //! Constructor with mesh pointer and generator properties
  StandardPointGenerator(const std::shared_ptr<mpm::Mesh<Tdim>>& mesh,
                 const Json& generator);

  //! Generate material points
  std::vector<Eigen::Matrix<double, Tdim, 1>> generate_points() override;

};  // StandardPointGenerator class
}  // namespace mpm

// #include "pointgenerator.tcc"

#endif  // MPM_POINTGENERATOR_H_
