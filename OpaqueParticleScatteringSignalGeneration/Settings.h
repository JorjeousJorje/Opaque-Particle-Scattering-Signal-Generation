#pragma once
#include <string_view>
#include "Config.h"
#include "SignalGenerator.h"


constexpr inline std::string_view filePathParametersP0{ "(amp)(p=0)(m=1.343).dat" };
constexpr inline std::string_view filePathParametersP21P22{ "(amp)(p=2.1 p=2.2)(m=1.343).dat" };
constexpr inline std::string_view filePathConfig{ "config.txt" };




static inline std::vector<ConfigItem> order{
	{"d", ValueId::Number},
	{"v", ValueId::Number},
	{"w0", ValueId::Number},
	{"output file", ValueId::PathToFile}
};

static inline Config config{ order };

constexpr double thetaScattering = 165.0;
constexpr double tStart = -15.0;
constexpr double tEnd = 15.0;

constexpr Polarization pol = Polarization::one;