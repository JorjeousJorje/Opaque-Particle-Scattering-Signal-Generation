#pragma once
#include "ScatteringOrderParameters.h"


constexpr inline std::string_view filePathParametersP0{ "(amp)(p=0)(m=1.343).dat" };
constexpr inline std::string_view filePathParametersP21P22{ "(amp)(p=2.1 p=2.2)(m=1.343).dat" };


constexpr double thetaScattering = 165.0;
constexpr double tStart = -5;
constexpr double tEnd = 10.0;

constexpr Polarization pol = Polarization::one;