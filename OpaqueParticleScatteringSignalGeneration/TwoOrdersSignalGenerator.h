#pragma once
#include "OneOrderSignalGenerator.h"

struct TwoOrderParameters {
	ScatteringOrderParameters params0;
	ScatteringOrderParameters params1;
};

class TwoOrdersSignalGenerator: public OneOrderSignalGenerator {
protected:
	ScatteringOrderParameters params1;
	OneOrderSignalGenerator _signalGen1;

public:
	explicit TwoOrdersSignalGenerator(const TwoOrderParameters& iParams, const LaserParticleParameters& iLPParams)
	:	OneOrderSignalGenerator{ iParams.params0, iLPParams },
		params1{ iParams.params1 },
		_signalGen1{ params1, _laserParticleParams }
	{
	}

	SignalHolder generateSignal(const valVec& iTime, const Polarization& iPol) override {
		const auto oSignal0 = OneOrderSignalGenerator::generateSignal(iTime, iPol, true);
		const auto oSignal1 = _signalGen1.generateSignal(iTime, iPol, true);

		SignalHolder oHolder{{
				{_params.mode, {oSignal0, _to, _sigma}},
				{params1.mode, {oSignal1, _signalGen1._to, _signalGen1._sigma}}
		}};
		return oHolder;
	}


};