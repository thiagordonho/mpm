// Getting the quadratures for Tnquadratures = 1
template <>
inline Eigen::MatrixXd mpm::TriangleQuadrature<2, 1>::quadratures() const {
  Eigen::Matrix<double, 2, 1> quadratures;
  const double val_1_by_3 = 1. / 3.;
  quadratures(0, 0) = val_1_by_3;
  quadratures(1, 0) = val_1_by_3;

  return quadratures;
}

// Getting the weights for Tnquadratures = 1
template <>
inline Eigen::VectorXd mpm::TriangleQuadrature<2, 1>::weights() const {
  Eigen::VectorXd weights(1);
  weights(0) = 0.5;

  return weights;
}

// Getting the quadratures for Tnquadratures = 4
template <>
inline Eigen::MatrixXd mpm::TriangleQuadrature<2, 3>::quadratures() const {
  Eigen::Matrix<double, 2, 3> quadratures;
  const double val_1_by_6 = 1. / 6.;
  const double val_2_by_3 = 2. / 3.;

  quadratures(0, 0) = val_1_by_6;
  quadratures(1, 0) = val_1_by_6;
  quadratures(0, 1) = val_2_by_3;
  quadratures(1, 1) = val_1_by_6;
  quadratures(0, 2) = val_1_by_6;
  quadratures(1, 2) = val_2_by_3;

  return quadratures;
}

// Getting the weights for Tnquadratures = 4
template <>
inline Eigen::VectorXd mpm::TriangleQuadrature<2, 3>::weights() const {
  Eigen::VectorXd weights(3);
  const double val_1_by_6 = 1. / 6.;
  
  weights(0) = val_1_by_6;
  weights(1) = val_1_by_6;
  weights(2) = val_1_by_6;

  return weights;
}

// Getting the quadratures for Tnquadratures = 9
template <>
inline Eigen::MatrixXd mpm::TriangleQuadrature<2, 4>::quadratures() const {
  Eigen::Matrix<double, 2, 4> quadratures;
  const double val_1_by_3 = 1. / 3.;
  const double val_1_by_5 = 0.2;
  const double val_3_by_5 = 0.6;

  quadratures(0, 0) = val_1_by_3;
  quadratures(1, 0) = val_1_by_3;

  quadratures(0, 1) = val_1_by_5;
  quadratures(1, 1) = val_1_by_5;

  quadratures(0, 2) = val_1_by_5;
  quadratures(1, 2) = val_3_by_5;

  quadratures(0, 3) = val_3_by_5;
  quadratures(1, 3) = val_1_by_5;

  return quadratures;
}

// Getting the weights for Tnquadratures = 9
template <>
inline Eigen::VectorXd mpm::TriangleQuadrature<2, 4>::weights() const {
  Eigen::VectorXd weights(4);
  const double val_9_by_32 = 9. / 32.;
  const double val_25_by_96 = 25. / 96.;

  weights(0) = -val_9_by_32;
  weights(1) = val_25_by_96;
  weights(2) = val_25_by_96;
  weights(3) = val_25_by_96;

  return weights;
}
