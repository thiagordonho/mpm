// Getting the quadratures for Tnquadratures = 1
template <>
inline Eigen::MatrixXd mpm::TetrahedronQuadrature<3, 1>::quadratures() const {
  Eigen::Matrix<double, 3, 1> quadratures;
  quadratures(0, 0) = 0.;
  quadratures(1, 0) = 0.;
  quadratures(2, 0) = 0.;

  return quadratures;
}

// Getting the weights for Tnquadratures = 1
template <>
inline Eigen::VectorXd mpm::TetrahedronQuadrature<3, 1>::weights() const {
  Eigen::VectorXd weights(1);
  weights(0) = 0.5;

  return weights;
}

// Getting the quadratures for Tnquadratures = 3
template <>
inline Eigen::MatrixXd mpm::TetrahedronQuadrature<3, 3>::quadratures() const {
  Eigen::Matrix<double, 3, 3> quadratures;
  quadratures(0, 0) = -1. / std::sqrt(3.);
  quadratures(1, 0) = -1. / std::sqrt(3.);
  quadratures(2, 0) = -1. / std::sqrt(3.);

  quadratures(0, 1) = 1. / std::sqrt(3.);
  quadratures(1, 1) = -1. / std::sqrt(3.);
  quadratures(2, 1) = -1. / std::sqrt(3.);

  quadratures(0, 2) = 1. / std::sqrt(3.);
  quadratures(1, 2) = 1. / std::sqrt(3.);
  quadratures(2, 2) = -1. / std::sqrt(3.);

  return quadratures;
}

// Getting the weights for Tnquadratures = 3
template <>
inline Eigen::VectorXd mpm::TetrahedronQuadrature<3, 3>::weights() const {
  Eigen::VectorXd weights(3);
  weights(0) = 1.;
  weights(1) = 1.;
  weights(2) = 1.;

  return weights;
}

// Getting the quadratures for Tnquadratures = 4
template <>
inline Eigen::MatrixXd mpm::TetrahedronQuadrature<3, 4>::quadratures() const {
  Eigen::Matrix<double, 3, 4> quadratures;
  const double qpoint_a = std::sqrt(3. / 5.);

  // Along xi(0) = -1 plane
  // Line xi(1) = -1
  quadratures(0, 0) = -qpoint_a;
  quadratures(1, 0) = -qpoint_a;
  quadratures(2, 0) = -qpoint_a;

  quadratures(0, 1) = -qpoint_a;
  quadratures(1, 1) = -qpoint_a;
  quadratures(2, 1) = 0.;

  quadratures(0, 2) = -qpoint_a;
  quadratures(1, 2) = -qpoint_a;
  quadratures(2, 2) = qpoint_a;

  // Line xi(1) = 0
  quadratures(0, 3) = -qpoint_a;
  quadratures(1, 3) = 0.;
  quadratures(2, 3) = -qpoint_a;

  return quadratures;
}

// Getting the weights for Tnquadratures = 4
template <>
inline Eigen::VectorXd mpm::TetrahedronQuadrature<3, 4>::weights() const {
  Eigen::VectorXd weights(4);
  const double weight_b = 5. / 9.;
  const double weight_c = 8. / 9.;

  weights(0) = weight_b * weight_b * weight_b;
  weights(1) = weight_b * weight_b * weight_c;
  weights(2) = weight_b * weight_b * weight_b;
  weights(3) = weight_b * weight_c * weight_b;

  return weights;
}
