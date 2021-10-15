#pragma once
#include "TwoOrdersSignalGenerator.h"

struct ThreeOrderParameters: TwoOrderParameters {
	ScatteringOrderParameters params2;
};

class ThreeOrdersSignalGenerator: public TwoOrdersSignalGenerator {
protected:
	ScatteringOrderParameters params2;
	OneOrderSignalGenerator _signalGen2;

public:
	ThreeOrdersSignalGenerator(const ThreeOrderParameters& iParams, const LaserParticleParameters& iLPParams)
	:	TwoOrdersSignalGenerator{ TwoOrderParameters(iParams), iLPParams },
		params2{ iParams.params2 },
		_signalGen2{ params2, _laserParticleParams }
	{
	}

	SignalHolder generateSignal(const valVec& iTime, const Polarization& iPol) override {
		const auto oSignal0 = OneOrderSignalGenerator::generateSignal(iTime, iPol, true);
		const auto oSignal1 = _signalGen1.generateSignal(iTime, iPol, true);
		const auto oSignal2 = _signalGen2.generateSignal(iTime, iPol, true);

		SignalHolder holder{ {
			{_params.mode, oSignal0},
			{params1.mode, oSignal1},
			{params2.mode, oSignal2}
		}};
		return holder;
	}


};