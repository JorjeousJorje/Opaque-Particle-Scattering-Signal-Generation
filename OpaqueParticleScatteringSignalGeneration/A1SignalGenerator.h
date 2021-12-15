#pragma once
#include <iostream>
#include <string>
#include <unordered_set>

#include "Utility.h"

#include "ThreeOrdersSignalGenerator.h"



class A1SignalGenerator final: public ThreeOrdersSignalGenerator {


public:
	explicit A1SignalGenerator(const ThreeOrderParameters& iParams, const LaserParticleParameters& iLPParams)
	:	ThreeOrdersSignalGenerator{ iParams , iLPParams}
	{
		const std::unordered_set mods{	iParams.params0.mode,
										iParams.params1.mode,
										iParams.params2.mode };

		if (!mods.count(ScatteringMode::P0)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P0));
			std::cout << "<A1SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P1)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P1));
			std::cout << "<A1SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P31)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P31));
			std::cout << "<A1SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}
	}
};
