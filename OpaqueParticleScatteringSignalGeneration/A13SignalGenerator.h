#pragma once
#include <string>
#include <iostream>
#include <unordered_set>

#include "Utility.h"
#include "ThreeOrdersSignalGenerator.h"


class A13SignalGenerator final: public ThreeOrdersSignalGenerator {

public:
	explicit A13SignalGenerator(const ThreeOrderParameters& iParams,
								const LaserParticleParameters LPParams)
	:	ThreeOrdersSignalGenerator{ iParams , LPParams }
	{
		const std::unordered_set mods{ iParams.params0.mode, iParams.params1.mode, iParams.params2.mode };

		if(!mods.count(ScatteringMode::P0)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P0));
			std::cout << "<A13SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P21)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P21));
			std::cout << "<A13SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P22)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P22));
			std::cout << "<A13SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}
	}
};