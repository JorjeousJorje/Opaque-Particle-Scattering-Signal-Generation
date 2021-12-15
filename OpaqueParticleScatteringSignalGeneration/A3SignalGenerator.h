#pragma once
#include <iostream>
#include <string>
#include <unordered_set>

#include "TwoOrdersSignalGenerator.h"
#include "Utility.h"


class A3SignalGenerator final : public TwoOrdersSignalGenerator {


public:
	explicit A3SignalGenerator(const TwoOrderParameters& iParams, const LaserParticleParameters& iLPParams)
	:	TwoOrdersSignalGenerator{ iParams , iLPParams }
	{
		const std::unordered_set mods{	iParams.params0.mode,
										iParams.params1.mode };

		if (!mods.count(ScatteringMode::P0)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P0));
			std::cout << "<A3SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P1)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P1));
			std::cout << "<A3SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}
	}
};
