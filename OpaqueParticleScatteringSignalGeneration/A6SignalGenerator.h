#pragma once
#include <iostream>
#include <string>
#include <unordered_set>

#include "OneOrderSignalGenerator.h"
#include "Utility.h"


class A6SignalGenerator final : public OneOrderSignalGenerator {


public:
	explicit A6SignalGenerator(const ScatteringOrderParameters& iParams, const LaserParticleParameters& iLPParams)
	:	OneOrderSignalGenerator{ iParams , iLPParams }
	{
		const std::unordered_set mods{ iParams.mode };

		if (!mods.count(ScatteringMode::P0)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P0));
			std::cout << "<A6SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}
	}
};
