//======================================================================================
// Athena++ astrophysical MHD code
// Copyright (C) 2014 James M. Stone  <jmstone@princeton.edu>
//
// This program is free software: you can redistribute and/or modify it under the terms
// of the GNU General Public License (GPL) as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
// PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of GNU GPL in the file LICENSE included in the code
// distribution.  If not see <http://www.gnu.org/licenses/>.
//======================================================================================

// Primary header
#include "eos.hpp"

// Athena headers
#include "../fluid.hpp"               // Fluid
#include "../../athena.hpp"           // enums, macros, Real
#include "../../athena_arrays.hpp"    // AthenaArray
#include "../../mesh.hpp"             // MeshBlock
#include "../../parameter_input.hpp"  // GetReal()
#include "../../field/field.hpp"      // BFields

//======================================================================================
//! \file isothermal_mhd.cpp
//  \brief implements functions in class FluidEqnOfState for isothermal MHD
//======================================================================================

// FluidEqnOfState constructor

FluidEqnOfState::FluidEqnOfState(Fluid *pf, ParameterInput *pin)
{
  pmy_fluid_ = pf;
  iso_sound_speed_ = pin->GetReal("fluid","iso_sound_speed"); // error if missing!
}

// destructor

FluidEqnOfState::~FluidEqnOfState()
{
}

//--------------------------------------------------------------------------------------
// \!fn void FluidEqnOfState::ConservedToPrimitive(AthenaArray<Real> &cons,
//   AthenaArray<Real> &prim_old, AthenaArray<Real> &prim)
// \brief convert conserved to primitive variables for adiabatic MHD

void FluidEqnOfState::ConservedToPrimitive(const AthenaArray<Real> &cons,
  const InterfaceBField &bi, const AthenaArray<Real> &prim_old,
  AthenaArray<Real> &prim, AthenaArray<Real> &bc)
{
  MeshBlock *pmb = pmy_fluid_->pmy_block;
  int jl = pmb->js; int ju = pmb->je;
  int kl = pmb->ks; int ku = pmb->ke;
  if (pmb->block_size.nx2 > 1) {
    jl -= (NGHOST);
    ju += (NGHOST);
  }
  if (pmb->block_size.nx3 > 1) {
    kl -= (NGHOST);
    ku += (NGHOST);
  }

//--------------------------------------------------------------------------------------
// Convert to Primitives

  for (int k=kl; k<=ku; ++k){
  for (int j=jl; j<=ju; ++j){
#pragma simd
    for (int i=pmb->is-(NGHOST); i<=pmb->ie+(NGHOST); ++i){
      const Real& u_d  = cons(IDN,k,j,i);
      const Real& u_m1 = cons(IVX,k,j,i);
      const Real& u_m2 = cons(IVY,k,j,i);
      const Real& u_m3 = cons(IVZ,k,j,i);

      Real di = 1.0/u_d;
      prim(IDN,k,j,i) = u_d;
      prim(IVX,k,j,i) = u_m1*di;
      prim(IVY,k,j,i) = u_m2*di;
      prim(IVZ,k,j,i) = u_m3*di;

      const Real& b1_i   = bi.x1(k,j,i  );
      const Real& b1_ip1 = bi.x1(k,j,i+1);
      const Real& b2_j   = bi.x2(k,j  ,i);
      const Real& b2_jp1 = bi.x2(k,j+1,i);
      const Real& b3_k   = bi.x3(k  ,j,i);
      const Real& b3_kp1 = bi.x3(k+1,j,i);

      bc(0,k,j,i) = 0.5*(b1_i + b1_ip1);
      bc(1,k,j,i) = 0.5*(b2_j + b2_jp1);
      bc(2,k,j,i) = 0.5*(b3_k + b3_kp1);
    }
  }}

  return;
}

//--------------------------------------------------------------------------------------
// \!fn Real FluidEqnOfState::SoundSpeed(Real dummy_arg[NFLUID])
// \brief returns isothermal sound speed -- does not need primitives 

Real FluidEqnOfState::SoundSpeed(const Real dummy_arg[NFLUID])
{
  return iso_sound_speed_;
}

//--------------------------------------------------------------------------------------
// \!fn Real FluidEqnOfState::FastMagnetosonicSpeed()
// \brief returns fast magnetosonic speed given vector of primitive variables
// Note the formula for (C_f)^2 is positive definite, so this func never returns a NaN

Real FluidEqnOfState::FastMagnetosonicSpeed(const Real bx,
  const Real prim[(NFLUID+NFIELD-1)])
{
  Real asq = (iso_sound_speed_*iso_sound_speed_);
  Real vaxsq = bx*bx/prim[IDN];
  Real ct2 = (prim[IBY]*prim[IBY] + prim[IBZ]*prim[IBZ])/prim[IDN];
  Real qsq = vaxsq + ct2 + asq;
  Real tmp = vaxsq + ct2 - asq;
  return sqrt(0.5*(qsq + sqrt(tmp*tmp + 4.0*asq*ct2)));
}
