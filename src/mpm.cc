#include <memory>

#include "factory.h"
#include "io.h"
#include "mpm.h"
#include "solvers/mpm_explicit.h"
#include "solvers/mpm_fsm_twophase.h"

// 2D Explicit MPM USF
static Register<mpm::MPM, mpm::MPMExplicit<2>, std::unique_ptr<mpm::IO>&&>
    mpm_explicit_2d("MPMExplicit2D");

// 3D Explicit MPM USF
static Register<mpm::MPM, mpm::MPMExplicit<3>, std::unique_ptr<mpm::IO>&&>
    mpm_explicit_3d("MPMExplicit3D");

// 2D Fractional Step Method Two Phase MPM
static Register<mpm::MPM, mpm::MPMFSMTwoPhase<2>, std::unique_ptr<mpm::IO>&&>
    mpm_fsm_twophase_2d("MPMFSMTwoPhase2D");

// 3D Fractional Step Method Two Phase MPM
static Register<mpm::MPM, mpm::MPMFSMTwoPhase<3>, std::unique_ptr<mpm::IO>&&>
    mpm_fsm_twophase_3d("MPMFSMTwoPhase3D");
