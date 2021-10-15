#pragma once

#include <iostream>
#include <unordered_set>
#include "ThreeOrdersSignalGenerator.h"


class A1SignalGenerator final: public ThreeOrdersSignalGenerator {


public:
	A1SignalGenerator(const ThreeOrderParameters& iParams, const LaserParticleParameters& iLPParams)
	: ThreeOrdersSignalGenerator{ iParams , iLPParams}
	{
		const std::unordered_set mods{ iParams.params0.mode, iParams.params1.mode, iParams.params2.mode };

		if (!mods.count(ScatteringMode::P0)) {
			std::cout << "<A1SignalGenerator>: there is no P0 mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P1)) {
			std::cout << "<A1SignalGenerator>: there is no P1 mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P31)) {
			std::cout << "<A1SignalGenerator>: there is no P31 mode, possibly result signal is wrong!" << std::endl;
		}
	}
};
