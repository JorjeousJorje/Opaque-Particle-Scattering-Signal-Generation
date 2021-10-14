#pragma once
#include <string_view>
#include "LaserParticleConfig.h"
#include "SignalGenerator.h"


constexpr inline std::string_view filePathParametersP0{ "(amp)(p=0)(m=1.343).dat" };
constexpr inline std::string_view filePathParametersP21P22{ "(amp)(p=2.1 p=2.2)(m=1.343).dat" };
constexpr inline std::string_view filePathLaserConfig{ "config.txt" };

static inline LaserParticleConfig config{};

constexpr double thetaScattering = 165.0;
constexpr double tStart = 0.0;
constexpr double tEnd = 15.0;

constexpr Polarization pol = Polarization::one;