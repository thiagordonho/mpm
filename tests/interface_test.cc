#include <limits>

#include "catch.hpp"

#include "nodal_properties.h"

#include "cell.h"
#include "element.h"
#include "function_base.h"
#include "hdf5_particle.h"
#include "hexahedron_element.h"
#include "linear_function.h"
#include "material.h"
#include "node.h"
#include "particle.h"
#include "quadrilateral_element.h"

//! \brief Check interface functions
TEST_CASE("Interface functions are checked", "[interface]") {
  // Number of materials
  const unsigned Nmaterials = 2;
  // Number of nodes
  const unsigned Nnodes = 4;
  // Number of phases
  const unsigned Nphases = 1;
  // Dimension
  const unsigned Dim = 2;
  // Degrees of freedom
  const unsigned Dof = 2;
  // Tolerance
  const double tolerance = 1.E-7;
  // Initialise a nodal property pool
  auto nodal_properties = std::make_shared<mpm::NodalProperties>();

  // Add all properties to the nodal properties pool
  nodal_properties->create_property("masses", Nnodes, Nmaterials);
  nodal_properties->create_property("momenta", Nnodes * Dim, Nmaterials);
  nodal_properties->create_property("velocities", Nnodes * Dim, Nmaterials);
  nodal_properties->create_property("current_velocities", Nnodes * Dim,
                                    Nmaterials);
  nodal_properties->create_property("accelerations", Nnodes * Dim, Nmaterials);
  nodal_properties->create_property("change_in_momenta", Nnodes * Dim,
                                    Nmaterials);
  nodal_properties->create_property("relative_velocities", Nnodes * Dim,
                                    Nmaterials);
  nodal_properties->create_property("domains", Nnodes, Nmaterials);
  nodal_properties->create_property("domain_gradients", Nnodes * Dim,
                                    Nmaterials);
  nodal_properties->create_property("normal_unit_vectors", Nnodes * Dim,
                                    Nmaterials);
  nodal_properties->create_property("internal_forces", Nnodes * Dim,
                                    Nmaterials);
  nodal_properties->create_property("external_forces", Nnodes * Dim,
                                    Nmaterials);
  nodal_properties->create_property("rigid_constraints", Nnodes, Nmaterials);

  // Element
  std::shared_ptr<mpm::Element<Dim>> element =
      std::make_shared<mpm::QuadrilateralElement<Dim, 4>>();

  // Create cell
  auto cell = std::make_shared<mpm::Cell<Dim>>(1, Nnodes, element);

  // Create nodes
  Eigen::Vector2d coords;
  coords << 0.0, 0.0;
  std::shared_ptr<mpm::NodeBase<Dim>> node0 =
      std::make_shared<mpm::Node<Dim, Dof, Nphases>>(0, coords);

  coords << 0.5, 0.0;
  std::shared_ptr<mpm::NodeBase<Dim>> node1 =
      std::make_shared<mpm::Node<Dim, Dof, Nphases>>(1, coords);

  coords << 0.0, 0.5;
  std::shared_ptr<mpm::NodeBase<Dim>> node2 =
      std::make_shared<mpm::Node<Dim, Dof, Nphases>>(3, coords);

  coords << 0.5, 0.5;
  std::shared_ptr<mpm::NodeBase<Dim>> node3 =
      std::make_shared<mpm::Node<Dim, Dof, Nphases>>(2, coords);

  // Create vector of nodes
  std::vector<std::shared_ptr<mpm::NodeBase<Dim>>> nodes;
  nodes.emplace_back(node0);
  nodes.emplace_back(node1);
  nodes.emplace_back(node3);
  nodes.emplace_back(node2);

  // Add nodes to cell
  for (unsigned i = 0; i < Nnodes; ++i) cell->add_node(i, nodes[i]);

  // Initialise cell properties
  cell->initialise();

  // Initialise property handle in each node
  for (unsigned i = 0; i < Nnodes; ++i)
    REQUIRE_NOTHROW(nodes[i]->initialise_property_handle(i, nodal_properties));

  // Create particle
  // Particle coordinates
  coords << 0.1, 0.2;
  auto particle1 = std::make_shared<mpm::Particle<Dim>>(0, coords);
  coords << 0.2, 0.1;
  auto particle2 = std::make_shared<mpm::Particle<Dim>>(1, coords);
  coords << 0.4, 0.4;
  auto particle3 = std::make_shared<mpm::Particle<Dim>>(2, coords);

  // Create vector of particles
  std::vector<std::shared_ptr<mpm::Particle<Dim>>> particles;
  particles.emplace_back(particle1);
  particles.emplace_back(particle2);
  particles.emplace_back(particle3);
  int Nparticles = particles.size();

  // Assign particle to cell
  for (unsigned i = 0; i < Nparticles; ++i) {
    particles[i]->assign_cell_id(1);
    particles[i]->assign_cell(cell);
  }

  // Initialise material
  Json jmaterial;
  jmaterial["density"] = 1000.;
  jmaterial["youngs_modulus"] = 1.0E+7;
  jmaterial["poisson_ratio"] = 0.3;

  auto material1 =
      Factory<mpm::Material<Dim>, unsigned, const Json&>::instance()->create(
          "LinearElastic2D", std::move(0), jmaterial);
  auto material2 =
      Factory<mpm::Material<Dim>, unsigned, const Json&>::instance()->create(
          "LinearElastic2D", std::move(1), jmaterial);

  // Assign materials to particles
  particles[0]->assign_material(material1);
  particles[1]->assign_material(material2);
  particles[2]->assign_material(material2);

  // Assign mass
  particles[0]->assign_mass(2.0);
  particles[1]->assign_mass(3.0);
  particles[2]->assign_mass(0.5);

  // Assign volume
  particles[0]->assign_volume(4.0);
  particles[1]->assign_volume(3.0);
  particles[2]->assign_volume(0.5);

  // Assign velocity
  Eigen::Vector2d velocity1;
  Eigen::Vector2d velocity2;
  for (unsigned i = 0; i < velocity1.size(); ++i) {
    velocity1(i) = i + 1;
    velocity2(i) = i - 0.5;
  }
  particles[0]->assign_velocity(velocity1);
  particles[1]->assign_velocity(velocity2);
  particles[2]->assign_velocity(velocity1);

  // Compute shape functions
  for (unsigned i = 0; i < Nparticles; ++i) particles[i]->compute_shapefn();

  // Append material ids to node
  for (unsigned i = 0; i < 2; ++i)
    for (unsigned j = 0; j < Nnodes; ++j)
      REQUIRE_NOTHROW(nodes[j]->append_material_id(i));

  // Check that all material ids were appended to node
  for (unsigned i = 0; i < 2; ++i)
    for (unsigned j = 0; j < Nnodes; ++j)
      REQUIRE(nodes[j]->material_ids().find(i) !=
              nodes[j]->material_ids().end());

  // Check computation of nodal mass, momentum, and velocity
  SECTION("Check mass, momentum, and velocity at nodes") {
    for (unsigned i = 0; i < Nparticles; ++i) {
      // Map masses and momenta from particles to nodes
      particles[i]->map_mass_momentum_to_nodes();

      // Map multimaterial properties from the particles to the nodes
      REQUIRE_NOTHROW(particles[i]->map_multimaterial_mass_momentum_to_nodes());
    }

    for (unsigned i = 0; i < Nnodes; ++i) {
      // Compute velocities at nodes
      nodes[i]->compute_velocity();

      // Compute multimaterial change in momentum
      REQUIRE_NOTHROW(nodes[i]->compute_multimaterial_change_in_momentum());
    }

    Eigen::Matrix<double, 4, 2> masses;
    // clang-format off
    masses << 0.96, 1.46,
              0.24, 1.04,
              0.16, 0.56,
              0.64, 0.44;
    // clang-format on

    Eigen::Matrix<double, 8, 2> momenta;
    // clang-format off
    momenta << 0.96,  -0.70,
               1.92,   0.76,
               0.24,  -0.40,
               0.48,   0.64,
               0.16,   0.20,
               0.32,   0.76,
               0.64,  -0.10,
               1.28,   0.34;
    // clang-format on

    Eigen::Matrix<double, 8, 2> delta_momenta;
    // clang-format off
    delta_momenta << -0.8568595041322, 0.8568595041322,
                     -0.8568595041322, 0.8568595041322,
                     -0.2700000000000, 0.2700000000000,
                     -0.2700000000000, 0.2700000000000,
                     -0.0800000000000, 0.0800000000000,
                     -0.0800000000000, 0.0800000000000,
                     -0.3200000000000, 0.3200000000000,
                     -0.3200000000000, 0.3200000000000;
    // clang-format on

    // Check values of mass and momentum at each node
    for (int i = 0; i < Nnodes; ++i) {
      for (int j = 0; j < Nmaterials; ++j) {
        REQUIRE(nodal_properties->property("masses", i, j, 1)(0, 0) ==
                Approx(masses(i, j)).epsilon(tolerance));
        for (int k = 0; k < Dim; ++k) {
          REQUIRE(nodal_properties->property("momenta", i, j, Dim)(k, 0) ==
                  Approx(momenta(i * Dim + k, j)).epsilon(tolerance));
          REQUIRE(nodal_properties->property("change_in_momenta", i, j, Dim)(
                      k, 0) ==
                  Approx(delta_momenta(i * Dim + k, j)).epsilon(tolerance));
        }
      }
    }

    // Check nodal velocity constraints
    SECTION("Check nodal velocity constraints") {
      // Create and apply the velocity constraints
      for (unsigned i = 0; i < Nnodes; ++i) {
        nodes[i]->assign_velocity_constraint(i % 2, 0.5 * i);
        REQUIRE_NOTHROW(nodes[i]->apply_contact_velocity_constraints());
      }

      Eigen::Matrix<double, 8, 2> velocities;
      // clang-format off
      velocities << 0.0, 0.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    0.5, 0.5,
                    1.0, 1.0,
                    0.0, 0.0,
                    0.0, 0.0,
                    1.5, 1.5;
      // clang-format on

      // Check values of mass and momentum at each node
      for (int i = 0; i < Nnodes; ++i)
        for (int j = 0; j < Nmaterials; ++j)
          for (int k = 0; k < Dim; ++k)
            REQUIRE(nodal_properties->property("velocities", i, j, Dim)(k, 0) ==
                    Approx(velocities(i * Dim + k, j)).epsilon(tolerance));
    }

    // Check nodal velocities from mapped momenta
    SECTION("Check nodal velocities from mapped momenta") {
      // Create velocity constraints and compute velocity from mass and momenta
      for (unsigned i = 0; i < Nnodes; ++i) {
        nodes[i]->assign_velocity_constraint(i % 2, 0.5 * i);
        REQUIRE_NOTHROW(nodes[i]->compute_multimaterial_velocity());
        REQUIRE_NOTHROW(nodes[i]->compute_multimaterial_relative_velocity());
      }

      Eigen::Matrix<double, 8, 2> velocities;
      // clang-format off
      velocities << 0.0,  0.0,
                    2.0,  0.520547945205479,
                    1.0, -0.384615384615385,
                    0.5,  0.5,
                    1.0,  1.0,
                    2.0,  1.35714285714286,
                    1.0, -0.227272727272727,
                    1.5,  1.5;
      // clang-format on

      Eigen::Matrix<double, 8, 2> rel_velocities;
      // clang-format off
      rel_velocities << -0.107438016528926, -0.107438016528926,
                         0.892561983471074, -0.586890071323446,
                         1.125000000000000, -0.259615384615385,
                        -0.375000000000000, -0.375000000000000,
                         0.500000000000000,  0.500000000000000,
                         0.500000000000000, -0.142857142857143,
                         0.500000000000000, -0.727272727272727,
                         0.000000000000000,  0.000000000000000;
      // clang-format on

      // Check values of mass and momentum at each node
      for (int i = 0; i < Nnodes; ++i) {
        for (int j = 0; j < Nmaterials; ++j) {
          for (int k = 0; k < Dim; ++k) {
            REQUIRE(nodal_properties->property("velocities", i, j, Dim)(k, 0) ==
                    Approx(velocities(i * Dim + k, j)).epsilon(tolerance));
            REQUIRE(nodal_properties->property("current_velocities", i, j, Dim)(
                        k, 0) ==
                    Approx(velocities(i * Dim + k, j)).epsilon(tolerance));
            REQUIRE(nodal_properties->property("relative_velocities", i, j,
                                               Dim)(k, 0) ==
                    Approx(rel_velocities(i * Dim + k, j)).epsilon(tolerance));
          }
        }
      }
    }

    SECTION("Check normal unit vector") {
      for (unsigned i = 0; i < Nparticles; ++i) {
        // Map multimaterial domains (volumes) to nodes
        REQUIRE_NOTHROW(particles[i]->map_multimaterial_domain_to_nodes());

        // Map multimaterial domain gradients to nodes
        REQUIRE_NOTHROW(
            particles[i]->map_multimaterial_domain_gradients_to_nodes());
      }

      // Compute normal unit vectors at nodes with PVG normal type
      for (unsigned i = 0; i < Nnodes; ++i)
        REQUIRE_NOTHROW(
            nodes[i]->compute_multimaterial_normal_unit_vector("PVG"));

      Eigen::Matrix<double, 4, 2> domains;
      // clang-format off
    domains << 1.92, 1.46,
              0.48, 1.04,
              0.32, 0.56,
              1.28, 0.44;
      // clang-format on

      Eigen::Matrix<double, 8, 2> gradients;
      // clang-format off
    gradients << -4.8, -5.0,
                 -6.4, -3.8,
                  4.8,  5.0,
                 -1.6, -3.2,
                  3.2,  2.0,
                  1.6,  3.2,
                 -3.2, -2.0,
                  6.4,  3.8;
      // clang-format on

      Eigen::Matrix<double, 8, 2> normal;
      // clang-format off
    normal << -0.60000000000000, -0.79616219412310,
              -0.80000000000000, -0.60508326753356,
               0.94868329805051,  0.84227140066151,
              -0.31622776601684, -0.53905369642337,
               0.89442719099992,  0.52999894000318,
               0.44721359549996,  0.84799830400509,
              -0.44721359549996, -0.46574643283262,
               0.89442719099992,  0.88491822238198;
      // clang-format on

      // Check if nodal properties were properly mapped and computed
      for (int i = 0; i < Nnodes; ++i) {
        for (int j = 0; j < Nmaterials; ++j) {
          REQUIRE(nodal_properties->property("domains", i, j, 1)(0, 0) ==
                  Approx(domains(i, j)).epsilon(tolerance));
          for (int k = 0; k < Dim; ++k) {
            REQUIRE(nodal_properties->property("domain_gradients", i, j, Dim)(
                        k, 0) ==
                    Approx(gradients(i * Dim + k, j)).epsilon(tolerance));
            REQUIRE(nodal_properties->property("normal_unit_vectors", i, j,
                                               Dim)(k, 0) ==
                    Approx(normal(i * Dim + k, j)).epsilon(tolerance));
          }
          // Check if normal vector are also unit vectors
          REQUIRE(nodal_properties->property("normal_unit_vectors", i, j, Dim)
                      .norm() == Approx(1.0).epsilon(tolerance));
        }
      }

      // Compute normal unit vectors at nodes with MVG normal type
      for (unsigned i = 0; i < Nnodes; ++i)
        REQUIRE_NOTHROW(
            nodes[i]->compute_multimaterial_normal_unit_vector("MVG"));

      // clang-format off
    normal << -0.60000000000000,  0.60000000000000,
              -0.80000000000000,  0.80000000000000,
              -0.84227140066151,  0.84227140066151,
               0.53905369642337, -0.53905369642337,
              -0.52999894000318,  0.52999894000318,
              -0.84799830400509,  0.84799830400509,
              -0.44721359549996,  0.44721359549996,
               0.89442719099992, -0.89442719099992;
      // clang-format on

      // Check if normal unit vector was properly computed
      for (int i = 0; i < Nnodes; ++i) {
        for (int j = 0; j < Nmaterials; ++j) {
          // Check if normal vector are also unit vectors
          REQUIRE(nodal_properties->property("normal_unit_vectors", i, j, Dim)
                      .norm() == Approx(1.0).epsilon(tolerance));
          for (int k = 0; k < Dim; ++k) {
            REQUIRE(nodal_properties->property("domain_gradients", i, j, Dim)(
                        k, 0) ==
                    Approx(gradients(i * Dim + k, j)).epsilon(tolerance));
            REQUIRE(nodal_properties->property("normal_unit_vectors", i, j,
                                               Dim)(k, 0) ==
                    Approx(normal(i * Dim + k, j)).epsilon(tolerance));
          }
        }
      }

      // Compute normal unit vectors at nodes with AVG normal type
      for (unsigned i = 0; i < Nnodes; ++i)
        REQUIRE_NOTHROW(
            nodes[i]->compute_multimaterial_normal_unit_vector("AVG"));

      // clang-format off
    normal << -0.273439154608222,  0.273439154608222,
              -0.961889301701158,  0.961889301701158,
              -0.749233543124942,  0.749233543124942,
               0.662305894475087, -0.662305894475087,
              -0.074789948241634,  0.074789948241634,
              -0.997199309888456,  0.997199309888456,
              -0.442365234368165,  0.442365234368165,
               0.896834990074762, -0.896834990074762;
      // clang-format on

      // Check if normal unit vector was properly computed
      for (int i = 0; i < Nnodes; ++i) {
        for (int j = 0; j < Nmaterials; ++j) {
          // Check if normal vector are also unit vectors
          REQUIRE(nodal_properties->property("normal_unit_vectors", i, j, Dim)
                      .norm() == Approx(1.0).epsilon(tolerance));
          for (int k = 0; k < Dim; ++k) {
            REQUIRE(nodal_properties->property("domain_gradients", i, j, Dim)(
                        k, 0) ==
                    Approx(gradients(i * Dim + k, j)).epsilon(tolerance));
            REQUIRE(nodal_properties->property("normal_unit_vectors", i, j,
                                               Dim)(k, 0) ==
                    Approx(normal(i * Dim + k, j)).epsilon(tolerance));
          }
        }
      }
    }

    // Check strain, stresses and internal forces
    SECTION("Check strain, stresses and internal forces") {
      // Map mass and momentum to nodes
      for (unsigned i = 0; i < Nparticles; ++i)
        REQUIRE_NOTHROW(
            particles[i]->map_multimaterial_mass_momentum_to_nodes());

      // Compute velocity at nodes
      for (unsigned i = 0; i < Nnodes; ++i)
        REQUIRE_NOTHROW(nodes[i]->compute_multimaterial_velocity());

      // Compute strain and stress at particles
      for (unsigned i = 0; i < Nparticles; ++i) {
        REQUIRE_NOTHROW(particles[i]->compute_strain(0.01, true));
        REQUIRE_NOTHROW(particles[i]->compute_stress());
      }

      Eigen::Matrix<double, 6, 3> strain_rates;
      // clang-format off
    strain_rates << 0.0, 0.385504906052851, 0.97299960313659,
                    0.0, 0.896021786432745, 1.2876849178219,
                    0.0, 0.000000000000000, 0.0,
                    0.0, 1.281526692485600, 2.26068452095849,
                    0.0, 0.000000000000000, 0.0,
                    0.0, 0.000000000000000, 0.0;
      // clang-format on

      // Check strain_rate
      for (unsigned i = 0; i < 6; ++i)
        for (unsigned j = 0; j < Nparticles; ++j)
          REQUIRE(particles[j]->strain_rate()(i, 0) ==
                  Approx(strain_rates(i, j)).epsilon(tolerance));

      // Map internal forces
      for (unsigned i = 0; i < Nparticles; ++i)
        REQUIRE_NOTHROW(particles[i]->map_multimaterial_internal_force());

      Eigen::Matrix<double, 8, 2> internal_forces;
      // clang-format off
    internal_forces << 0.0,  733110.672257143,
                       0.0,  814167.128972187,
                       0.0, -350424.338569755,
                       0.0,  272463.574360308,
                       0.0, -476376.626534688,
                       0.0, -655149.908047695,
                       0.0,   93690.2928473004,
                       0.0, -431480.795284800;
      // clang-format on

      // Check internal forces
      for (int i = 0; i < Nnodes; ++i)
        for (int j = 0; j < Nmaterials; ++j)
          for (int k = 0; k < Dim; ++k)
            REQUIRE(nodal_properties->property("internal_forces", i, j, Dim)(
                        k, 0) ==
                    Approx(internal_forces(i * Dim + k, j)).epsilon(tolerance));
    }

    // Check external forces
    SECTION("Check external forces") {
      // Gravity
      Eigen::Vector2d gravity;
      gravity << 2.0, 8.0;

      // Assign traction to particle
      particles[1]->assign_traction(0, 2.5);
      particles[2]->assign_traction(1, -1.0);

      // Assign concentrated forces to nodes
      nodes[1]->assign_concentrated_force(0, 0, 2.0, nullptr);
      nodes[2]->assign_concentrated_force(0, 1, 5.0, nullptr);

      // Map body force and traction to nodes
      for (unsigned i = 0; i < Nparticles; ++i) {
        REQUIRE_NOTHROW(particles[i]->map_multimaterial_body_force(gravity));
        REQUIRE_NOTHROW(particles[i]->map_multimaterial_traction_force());
      }

      // Apply concentrated nodal forces
      for (unsigned i = 0; i < Nnodes; ++i)
        REQUIRE_NOTHROW(
            nodes[i]->apply_multimaterial_concentrated_force(0, 0.0));

      Eigen::Matrix<double, 8, 2> external_forces;
      // clang-format off
    external_forces << 1.92, 4.99846096908265,
                       7.68, 11.6517157287525,
                       2.48, 5.46564064605510,
                       1.92, 8.20686291501015,
                       0.32, 1.46641016151378,
                       6.28, 9.02745166004061,
                       1.28, 1.39961524227066,
                       5.12, 3.40686291501015;
      // clang-format on

      // Check external forces
      for (int i = 0; i < Nnodes; ++i)
        for (int j = 0; j < Nmaterials; ++j)
          for (int k = 0; k < Dim; ++k)
            REQUIRE(nodal_properties->property("external_forces", i, j, Dim)(
                        k, 0) ==
                    Approx(external_forces(i * Dim + k, j)).epsilon(tolerance));
    }

    // Check mapping of particle constrained status
    SECTION("Check mapping of particle constrained status") {
      // Apply particle velocity constraint to one particle
      particles[1]->apply_particle_velocity_constraints(0, 1.0);

      // Map particle constraint status to all nodes
      for (unsigned i = 0; i < Nparticles; ++i)
        REQUIRE_NOTHROW(particles[i]->map_multimaterial_rigid_constraint());

      Eigen::Matrix<double, 4, 2> constraints;
      // clang-format off
      constraints << 0.0, 1.0,
                     0.0, 1.0,
                     0.0, 1.0,
                     0.0, 1.0;
      // clang-format on

      // Check constraint status at nodes and materials
      for (int i = 0; i < Nnodes; ++i)
        for (int j = 0; j < Nmaterials; ++j)
          REQUIRE(nodal_properties->property("rigid_constraints", i, j, 1)(
                      0, 0) == Approx(constraints(i, j)).epsilon(tolerance));
    }

    // Check computation of acceleration and velocity at multimaterial nodes
    SECTION("Check computation of acceleration and velocity") {
      // Assign generic internal and external forces
      Eigen::Matrix<double, 2, 1> force;
      force << 1.0, -1.0;
      for (unsigned i = 0; i < Nnodes; ++i) {
        for (unsigned j = 0; j < 2; ++j) {
          REQUIRE_NOTHROW(nodal_properties->assign_property(
              "internal_forces", i, j, (0.5 * i - 2.0 * j) * force, Dim));
          REQUIRE_NOTHROW(nodal_properties->assign_property(
              "external_forces", i, j, (2.0 * i + 0.5 * j) * force, Dim));
        }
      }

      Eigen::Matrix<double, 8, 2> internals;
      // clang-format off
      internals <<  0.0, -2.0,
                    0.0,  2.0,
                    0.5, -1.5,
                   -0.5,  1.5,
                    1.0, -1.0,
                   -1.0,  1.0,
                    1.5, -0.5,
                   -1.5,  0.5;
      // clang-format on

      Eigen::Matrix<double, 8, 2> externals;
      // clang-format off
      externals <<  0.0,  0.5,
                    0.0, -0.5,
                    2.0,  2.5,
                   -2.0, -2.5,
                    4.0,  4.5,
                   -4.0, -4.5,
                    6.0,  6.5,
                   -6.0, -6.5;
      // clang-format on

      // Check acceleration and velocity after computations
      for (int i = 0; i < Nnodes; ++i) {
        for (int j = 0; j < Nmaterials; ++j) {
          for (int k = 0; k < Dim; ++k) {
            REQUIRE(nodal_properties->property("internal_forces", i, j, Dim)(
                        k, 0) ==
                    Approx(internals(i * Dim + k, j)).epsilon(tolerance));
            REQUIRE(nodal_properties->property("external_forces", i, j, Dim)(
                        k, 0) ==
                    Approx(externals(i * Dim + k, j)).epsilon(tolerance));
          }
        }
      }

      // Compute acceleration and velocity at every node
      for (int i = 0; i < Nnodes; ++i)
        REQUIRE(nodes[i]->compute_contact_acceleration_velocity(0.01) == true);

      Eigen::Matrix<double, 8, 2> accelerations;
      // clang-format off
      accelerations <<   0.0000000000000,  -1.02739726027397,
                         0.0000000000000,   1.02739726027397,
                        10.4166666666667,   0.961538461538461,
                       -10.4166666666667,  -0.961538461538461,
                        31.2500000000000,   6.25,
                       -31.2500000000000,  -6.25,
                        11.7187500000000,  13.6363636363636,
                       -11.7187500000000, -13.6363636363636;
      // clang-format on

      // Check acceleration and velocity after computations
      for (int i = 0; i < Nnodes; ++i) {
        for (int j = 0; j < Nmaterials; ++j) {
          for (int k = 0; k < Dim; ++k) {
            REQUIRE(nodal_properties->property("velocities", i, j, Dim)(k, 0) ==
                    Approx(0.01 * accelerations(i * Dim + k, j))
                        .epsilon(tolerance));
            REQUIRE(
                nodal_properties->property("accelerations", i, j, Dim)(k, 0) ==
                Approx(accelerations(i * Dim + k, j)).epsilon(tolerance));
          }
        }
      }

      // Assign zero mass
      Eigen::Matrix<double, 1, 1> mass;
      mass << 0.0;
      for (unsigned i = 0; i < Nnodes; ++i)
        for (unsigned j = 0; j < 2; ++j)
          REQUIRE_NOTHROW(
              nodal_properties->assign_property("masses", i, j, mass));

      // Check acceleration and velocity computation for no mass
      for (int i = 0; i < Nnodes; ++i)
        REQUIRE(nodes[i]->compute_contact_acceleration_velocity(0.01) == false);
    }

    // Check the application of contact mechanics
    SECTION("Check the application of contact mechanics") {
      // Create reference normal unit vector
      Eigen::Matrix<double, 2, 1> ref_normal;
      ref_normal << 0.8, 0.6;

      // Create reference velocity vector
      Eigen::Matrix<double, 2, 1> ref_velocity;
      ref_velocity << -0.5, -1.0;

      // Assign Current velocity, relative velocity and normal unit vectors
      for (unsigned i = 0; i < Nnodes; ++i) {
        for (unsigned j = 0; j < 2; ++j) {
          Eigen::Matrix<double, 2, 1> relative_velocity =
              (1.0 - 2.0 * j) * ref_normal * (1.0 + i) -
              ref_velocity * i * (j + 1.0);
          nodal_properties->assign_property("normal_unit_vectors", i, j,
                                            ref_normal * (1.0 - 2.0 * j), Dim);
          REQUIRE_NOTHROW(nodal_properties->assign_property(
              "relative_velocities", i, j, relative_velocity, Dim));
          relative_velocity(0, 0) = relative_velocity(0, 0) + (i + 1.0) * 0.5;
          relative_velocity(1, 0) = relative_velocity(1, 0) + (i + 1.0) * 0.5;
          REQUIRE_NOTHROW(nodal_properties->assign_property(
              "current_velocities", i, j, relative_velocity, Dim));
          REQUIRE_NOTHROW(nodal_properties->assign_property(
              "velocities", i, j, relative_velocity, Dim));
        }
      }

      // Apply contact mechanics to all nodes
      double mu = 0.2;
      double dt = 0.01;
      for (unsigned i = 0; i < Nnodes; ++i) {
        REQUIRE_NOTHROW(nodes[i]->apply_contact_mechanics(mu, dt));
      }

      Eigen::Matrix<double, 8, 2> velocities;
      // clang-format off
      velocities <<  0.50, 0.5,
                     0.50, 0.5,
                     1.00, 0.4,
                     1.00, 1.8,
                     1.50, 1.1,
                     1.50, 3.7,
                     1.94, 1.8,
                     2.08, 5.6;
      // clang-format on

      Eigen::Matrix<double, 8, 2> accelerations;
      // clang-format off
      accelerations <<  -80, 80,
                        -60, 60,
                       -210,  0,
                       -220,  0,
                       -340,  0,
                       -380,  0,
                       -476,  0,
                       -532,  0;
      // clang-format on

      // Check corrected velocities and accelerations
      for (int i = 0; i < Nnodes; ++i) {
        for (int j = 0; j < Nmaterials; ++j) {
          for (int k = 0; k < Dim; ++k) {
            REQUIRE(nodal_properties->property("velocities", i, j, Dim)(k, 0) ==
                    Approx(velocities(i * Dim + k, j)).epsilon(tolerance));
            REQUIRE(
                nodal_properties->property("accelerations", i, j, Dim)(k, 0) ==
                Approx(accelerations(i * Dim + k, j)).epsilon(tolerance));
          }
        }
      }
    }

    // Check particle update from multimaterial velocities
    SECTION("Check particle update from nodal velocities") {
      // Create generic nodal velocities
      double dt = 0.01;
      Eigen::Matrix<double, 2, 1> ref_vector1, ref_vector2;
      ref_vector1 << 1.0, 1.0;
      ref_vector2 << 1.0, -1.0;
      for (unsigned i = 0; i < Nnodes; ++i) {
        for (unsigned j = 0; j < 2; ++j) {
          Eigen::Matrix<double, 2, 1> accel;
          accel = ref_vector1 * (j + 1.0) + i * 0.5 * ref_vector2;
          REQUIRE_NOTHROW(nodal_properties->assign_property("velocities", i, j,
                                                            dt * accel, Dim));
        }
      }

      // Update particle positions with acceleration update
      for (unsigned i = 0; i < Nparticles; ++i)
        REQUIRE_NOTHROW(
            particles[i]->compute_contact_updated_position(dt, true));

      Eigen::Matrix<double, 2, 3> velocities;
      // clang-format off
      velocities << 0.0162,  0.0242, 0.0296,
                    0.0038,  0.0158, 0.0104;
      // clang-format on
      Eigen::Matrix<double, 2, 3> coordinates;
      // clang-format off
      coordinates << 0.100162, 0.200242, 0.400296,
                     0.200038, 0.100158, 0.400104;
      // clang-format on
      Eigen::Matrix<double, 2, 3> displacements;
      // clang-format off
      displacements << 0.000162, 0.000242, 0.000296,
                       0.000038, 0.000158, 0.000104;
      // clang-format on

      // Check velocities, displacements and coordinates at particles
      for (unsigned i = 0; i < Nparticles; ++i) {
        REQUIRE(particles[i]->velocity()(0, 0) ==
                Approx(velocities(0, i)).epsilon(tolerance));
        REQUIRE(particles[i]->velocity()(1, 0) ==
                Approx(velocities(1, i)).epsilon(tolerance));
        REQUIRE(particles[i]->coordinates()(0, 0) ==
                Approx(coordinates(0, i)).epsilon(tolerance));
        REQUIRE(particles[i]->coordinates()(1, 0) ==
                Approx(coordinates(1, i)).epsilon(tolerance));
        REQUIRE(particles[i]->displacement()(0, 0) ==
                Approx(displacements(0, i)).epsilon(tolerance));
        REQUIRE(particles[i]->displacement()(1, 0) ==
                Approx(displacements(1, i)).epsilon(tolerance));
      }
    }

    // Check particle update from multimaterial accelerations
    SECTION("Check particle update from nodal acceleration") {
      // Create generic nodal accelerations
      double dt = 0.01;
      Eigen::Matrix<double, 2, 1> ref_vector1, ref_vector2;
      ref_vector1 << 1.0, 1.0;
      ref_vector2 << 1.0, -1.0;
      for (unsigned i = 0; i < Nnodes; ++i) {
        for (unsigned j = 0; j < 2; ++j) {
          Eigen::Matrix<double, 2, 1> accel;
          accel = ref_vector1 * (j + 1.0) + i * 0.5 * ref_vector2;
          REQUIRE_NOTHROW(nodal_properties->assign_property("accelerations", i,
                                                            j, accel, Dim));
          REQUIRE_NOTHROW(nodal_properties->assign_property("velocities", i, j,
                                                            dt * accel, Dim));
        }
      }

      // Update particle positions with acceleration update
      for (unsigned i = 0; i < Nparticles; ++i)
        REQUIRE_NOTHROW(
            particles[i]->compute_contact_updated_position(dt, false));

      Eigen::Matrix<double, 2, 3> velocities;
      // clang-format off
      velocities << 1.0162, -0.4758, 1.0296,
                    2.0038,  0.5158, 2.0104;
      // clang-format on
      Eigen::Matrix<double, 2, 3> coordinates;
      // clang-format off
      coordinates << 0.100162, 0.200242, 0.400296,
                     0.200038, 0.100158, 0.400104;
      // clang-format on
      Eigen::Matrix<double, 2, 3> displacements;
      // clang-format off
      displacements << 0.000162, 0.000242, 0.000296,
                       0.000038, 0.000158, 0.000104;
      // clang-format on

      // Check velocities, displacements and coordinates at particles
      for (unsigned i = 0; i < Nparticles; ++i) {
        REQUIRE(particles[i]->velocity()(0, 0) ==
                Approx(velocities(0, i)).epsilon(tolerance));
        REQUIRE(particles[i]->velocity()(1, 0) ==
                Approx(velocities(1, i)).epsilon(tolerance));
        REQUIRE(particles[i]->coordinates()(0, 0) ==
                Approx(coordinates(0, i)).epsilon(tolerance));
        REQUIRE(particles[i]->coordinates()(1, 0) ==
                Approx(coordinates(1, i)).epsilon(tolerance));
        REQUIRE(particles[i]->displacement()(0, 0) ==
                Approx(displacements(0, i)).epsilon(tolerance));
        REQUIRE(particles[i]->displacement()(1, 0) ==
                Approx(displacements(1, i)).epsilon(tolerance));
      }
    }
  }
}
