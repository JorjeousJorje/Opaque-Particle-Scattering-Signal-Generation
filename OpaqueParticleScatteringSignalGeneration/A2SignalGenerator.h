#pragma once
#include <iostream>
#include <unordered_set>
#include "FourOrdersSignalGenerator.h"


class A2SignalGenerator final : public FourOrdersSignalGenerator {


public:
	A2SignalGenerator(const FourOrdersParameters& iParams, const LaserParticleParameters& iLPParams)
		: FourOrdersSignalGenerator{ iParams , iLPParams }
	{
		const std::unordered_set mods{ iParams.params0.mode, iParams.params1.mode, iParams.params2.mode, iParams.params3.mode };

		if (!mods.count(ScatteringMode::P0)) {
			std::cout << "<A1SignalGenerator>: there is no P0 mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P1)) {
			std::cout << "<A1SignalGenerator>: there is no P1 mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P31)) {
			std::cout << "<A1SignalGenerator>: there is no P31 mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P32)) {
			std::cout << "<A1SignalGenerator>: there is no P32 mode, possibly result signal is wrong!" << std::endl;
		}
	}
};
