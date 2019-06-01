#include "polynomial.h"

//! 2D Quadrilateral
//! Polynomial order 1 and number of terms 4
//! 4 monomials: 1, y, x, xy
//! Integration of monomials over [-1,-1] x [1,1] unit square
template <>
const Eigen::Matrix<double, 4, 1>
mpm::Polynomial::IntegralBase<2, 1, 4>::Square_Definite_Integrals =
        (Eigen::MatrixXd(4, 1) << 4.0, 0., 0., 0.).finished();

//! 2D Quadrilateral
//! Polynomial order 2 and number of terms 9
//! 9 monomials: 1, y, y^2, x, xy, xy^2, x^2, x^2 y, x^2 y^2
//! Integration of monomials over [-1,-1] x [1,1] unit square
template <>
const Eigen::Matrix<double, 9, 1>
mpm::Polynomial::IntegralBase<2, 2, 9>::Square_Definite_Integrals =
        (Eigen::MatrixXd(9, 1) << 4.0, 0., 4. / 3, 0., 0., 0., 4. / 3, 0.,
         4. / 9)
            .finished();

//! 2D Quadrilateral
//! Polynomial order 3 and number of terms 16
//! 16 monomials: 1, y, y^2, y^3, x, xy, xy^2, xy^3, x^2, x^2 y, x^2 y^2, x^2
//! y^3, x^3, x^3 y, x^3 y^2, x^3 y^3
//! Integration of monomials over [-1,-1] x [1,1] unit square
template <>
const Eigen::Matrix<double, 16, 1>
mpm::Polynomial::IntegralBase<2, 3, 16>::Square_Definite_Integrals =
        (Eigen::MatrixXd(16, 1) << 4.0, 0., 4. / 3, 0., 0., 0., 0., 0., 4. / 3,
         0., 4. / 9, 0., 0., 0., 0., 0.)
            .finished();

//! 3D Hexahedron
//! Polynomial order 2 and number of terms 9
//! 27 monomials: 1, z, z^2, y, yz, yz^2, y^2, y^2 z, y^2 z^2, x, xz, x z^2, x
//! y, xyz, xyz^2, xy^2, xy^2 z, xy^2 z^2, x^2, x^2 z, x^2 z^2, x^2 y, x^2 yz,
//! x^2 yz^2, x^2 y^2, y^2 z, x^2 y^2 z^2
//! Integration of monomials over [-1,-1, -1] x [1,1, 1] unit cube
template <>
const Eigen::Matrix<double, 27, 1>
mpm::Polynomial::IntegralBase<3, 2, 27>::Square_Definite_Integrals = Eigen::Matrix<double, 27, 1>::Zero();
