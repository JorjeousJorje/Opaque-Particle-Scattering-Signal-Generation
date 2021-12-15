#pragma once
#include <iostream>
#include <string>
#include <unordered_set>

#include "FourOrdersSignalGenerator.h"
#include "Utility.h"


class A12SignalGenerator final : public FourOrdersSignalGenerator {


public:
	A12SignalGenerator(const FourOrdersParameters& iParams, const LaserParticleParameters& iLPParams)
		: FourOrdersSignalGenerator{ iParams , iLPParams }
	{
		const std::unordered_set mods{ iParams.params0.mode,
										iParams.params1.mode,
										iParams.params2.mode,
										iParams.params3.mode };

		if (!mods.count(ScatteringMode::P0)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P0));
			std::cout << "<A12SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P21)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P21));
			std::cout << "<A12SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P22)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P22));
			std::cout << "<A12SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P23)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P23));
			std::cout << "<A12SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}
	}
};