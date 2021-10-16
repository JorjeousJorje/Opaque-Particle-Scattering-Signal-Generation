#pragma once
#include <iostream>
#include <string>
#include <unordered_set>

#include "TwoOrdersSignalGenerator.h"
#include "Utility.h"


class A4SignalGenerator final : public TwoOrdersSignalGenerator {


public:
	A4SignalGenerator(const TwoOrderParameters& iParams, const LaserParticleParameters& iLPParams)
	:	TwoOrdersSignalGenerator{ iParams , iLPParams }
	{
		const std::unordered_set mods{	iParams.params0.mode,
										iParams.params1.mode };

		if (!mods.count(ScatteringMode::P0)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P0));
			std::cout << "<A1SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P31)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P31));
			std::cout << "<A1SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}
	}
};
