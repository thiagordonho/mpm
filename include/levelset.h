#ifndef MPM_LEVELSET_H_
#define MPM_LEVELSET_H_

#include <vector>
#include <string>

#include "Eigen/Dense"

#include "logger.h"
#include "polynomial.h"

namespace mpm {

// Integration domain is decided based on the level set value, "phi"
//! Integration_In: Integration is done for phi < 0
//! Integration_Out: Integration is doen for phi > 0
//! Boundary: Integration is done for phi = 0
enum class IntegrationDomain { Integration_In, Integration_Out, Boundary };

//! Base class for boundary representation using level set function
//! The level set function is described using a polynomial
//! tparam Tdim Dimension
template <unsigned Tdim>
class LevelSet {
public:
 //! Define a vector of size dimension
 using VectorDim = Eigen::Matrix<double, Tdim, 1>;

 //! Constructor with id and polynomial coefficients
 //! \param[in] id level set id for the boundary
 //! \param[in] domain Integration domain
 //! \param[in] moving_status Moving or Fixed
 LevelSet(unsigned id, const std::string& domain, bool moving_status);

 //! Default destructor
 ~LevelSet() = default;

 //! Delete copy constructor
 LevelSet(const LevelSet<Tdim>&) = delete;

 //! Delete assignement operator
 LevelSet& operator=(const LevelSet<Tdim>&) = delete;

 // Assign the polynomial function
 //! \param[in] porder Polynomial order
 //! \param[in] coefficients coefficients of the polynomial
 void assign_polynomial_function(unsigned porder,
                                 std::vector<double>& coefficients) {
   poly_order_ = porder;
   poly_coefficients_ = coefficients;
 }

 // Return id
 unsigned id() const { return id_; }

 // Return moving status
 bool moving() { return moving_; }


private:
 // level set id
 unsigned id_;
 // integration domain
 mpm::IntegrationDomain integration_domain_{
     mpm::IntegrationDomain::Integration_In};
 // polynomial order
 unsigned poly_order_;
 // polyomial coefficient vector
 std::vector<double> poly_coefficients_;
 // status (fixed or moving)
 bool moving_{false};
 //! Logger
 std::unique_ptr<spdlog::logger> console_;
};

}  // namespace mpm

#include "levelset.tcc"

#endif 
