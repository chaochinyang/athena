//========================================================================================
// Athena++ astrophysical MHD code
// Copyright(C) 2026 James M. Stone <jmstone@princeton.edu> and other code contributors
// Licensed under the 3-clause BSD License, see LICENSE file for details
//========================================================================================
//! \file int2d.cpp
//! \brief implements non-trivial functions of IntX[123]X[123]Output classes.

// C/C++ headers

// Athena++ headers
#include "outputs.hpp"

//----------------------------------------------------------------------------------------
//! \fn void IntX1X2Output::WriteOutputFile(Mesh *pm, ParameterInput *pin, bool flag)
//! \brief integrates the data over x1 and x2 directions and writes the resulting 1D
//      array in x3.

void IntX1X2Output::WriteOutputFile(Mesh *pm, ParameterInput *pin, bool flag) {
}

//----------------------------------------------------------------------------------------
//! \fn void IntX1X3Output::WriteOutputFile(Mesh *pm, ParameterInput *pin, bool flag)
//! \brief integrates the data over x1 and x3 directions and writes the resulting 1D
//      array in x2.

void IntX1X3Output::WriteOutputFile(Mesh *pm, ParameterInput *pin, bool flag) {
}

//----------------------------------------------------------------------------------------
//! \fn void IntX2X3Output::WriteOutputFile(Mesh *pm, ParameterInput *pin, bool flag)
//! \brief integrates the data over x2 and x3 directions and writes the resulting 1D
//      array in x1.

void IntX2X3Output::WriteOutputFile(Mesh *pm, ParameterInput *pin, bool flag) {
}
