#ifndef MPM_LOAD_BASE_H_
#define MPM_LOAD_BASE_H_

// Eigen
#include "Eigen/Dense"

#include "mesh.h"

namespace mpm {

//! Load Base class
//! \brief Base class that handles external loading
//! \details load base class to apply external loads on nodes and particles
//! \tparam Tdim Dimension
template <unsigned Tdim>
class LoadBase {
 public:
  //! Define a vector of size dimension
  using VectorDim = Eigen::Matrix<double, Tdim, 1>;

  // Construct a Load Base with a global unique id
  //! \param[in] id Global id
  LoadBase(unsigned id){};

  //! Default destructor
  ~LoadBase() = default;

  //! Delete copy constructor
  LoadBase(const LoadBase<Tdim>&) = delete;

  //! Delete assignement operator
  LoadBase& operator=(const LoadsBase<Tdim>&) = delete;

  virtual double value(const double current_time,
                       const double magnitude) const = 0;
};  // DofConstraintsBase class
}  // namespace mpm


#endif  // MPM_LOAD_BASE_H_
