#ifndef MPM_POLYNOMIAL_H_
#define MPM_POLYNOMIAL_H_

#include <vector>

#include "Eigen/Dense"

#include "logger.h"

namespace mpm {

namespace Polynomial {

//! Base class that stores the definite integrals of monomials
//! over reference element
//! tparam Tdim Dimension
//! tparam Tporder Polynomial order
template <unsigned Tdim, unsigned Tporder, unsigned Tnterms>
struct IntegralBase {
  virtual Eigen::Matrix<double, Tnterms, 1> definite_integrals() const = 0;
  static const Eigen::Matrix<double, Tnterms, 1> Square_Definite_Integrals;
  static const Eigen::Matrix<double, Tnterms, 1> Tri_Definite_Integrals;
};

} // namespace Polynomial
} // namespace mpm

#endif 
