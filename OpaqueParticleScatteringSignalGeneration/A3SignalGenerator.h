#pragma once
#include <iostream>
#include <unordered_set>
#include "TwoOrdersSignalGenerator.h"


class A3SignalGenerator final : public TwoOrdersSignalGenerator {


public:
	A3SignalGenerator(const TwoOrderParameters& iParams, const LaserParticleParameters& iLPParams)
		: TwoOrdersSignalGenerator{ iParams , iLPParams }
	{
		const std::unordered_set mods{ iParams.params0.mode, iParams.params1.mode };

		if (!mods.count(ScatteringMode::P0)) {
			std::cout << "<A1SignalGenerator>: there is no P0 mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P1)) {
			std::cout << "<A1SignalGenerator>: there is no P1 mode, possibly result signal is wrong!" << std::endl;
		}
	}
};
