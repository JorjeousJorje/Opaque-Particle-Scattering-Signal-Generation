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

	void setParams(const TwoOrderParameters& iParams) {
		_params = iParams.params0;
		_signalGen1.setParams(iParams.params1);
		params1 = iParams.params1;
	}
	void setLaserParams(const LaserParticleParameters& iParams) override {
		_laserParticleParams = iParams;
		_signalGen1.setLaserParams(iParams);
	}


	SignalHolder generateSignal(const valVec& iTime, const Polarization& iPol) override {
		const auto oSignal0 = OneOrderSignalGenerator::generateSignal(iTime, iPol, true);
		const auto oSignal1 = _signalGen1.generateSignal(iTime, iPol, true);

		SignalHolder oHolder{{
				{_params.mode, {oSignal0, _to, _sigma, _amplitude}},
				{params1.mode, {oSignal1, _signalGen1._to, _signalGen1._sigma, _signalGen1._amplitude}}
		}};
		return oHolder;
	}


};