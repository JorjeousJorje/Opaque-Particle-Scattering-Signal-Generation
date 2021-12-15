#pragma once
#include <iostream>
#include <string>
#include <unordered_set>

#include "FiveOrdersSignalGenerator.h"
#include "Utility.h"


class A10SignalGenerator final : public FiveOrdersSignalGenerator {

public:
	explicit A10SignalGenerator(const FiveOrdersParameters& iParams,
								const LaserParticleParameters LPParams)
	:	FiveOrdersSignalGenerator{ iParams , LPParams }
	{
		const std::unordered_set mods{	iParams.params0.mode,
										iParams.params1.mode,
										iParams.params2.mode,
										iParams.params3.mode,
										iParams.params4.mode };

		if (!mods.count(ScatteringMode::P0)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P0));
			std::cout << "<A10SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P21)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P21));
			std::cout << "<A10SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P32)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P22));
			std::cout << "<A10SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P31)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P31));
			std::cout << "<A10SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}

		if (!mods.count(ScatteringMode::P32)) {
			const auto modeString = std::to_string(Utility::to_underlying(ScatteringMode::P32));
			std::cout << "<A10SignalGenerator>: there is no " + modeString + "mode, possibly result signal is wrong!" << std::endl;
		}
	}
};
