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
	explicit ThreeOrdersSignalGenerator(const ThreeOrderParameters& iParams, const LaserParticleParameters& iLPParams)
	:	TwoOrdersSignalGenerator{ TwoOrderParameters(iParams), iLPParams },
		params2{ iParams.params2 },
		_signalGen2{ params2, _laserParticleParams }
	{
	}


	void setParams(const ThreeOrderParameters& iParams) {
		TwoOrdersSignalGenerator::setParams(TwoOrderParameters(iParams));
		_signalGen2.setParams(iParams.params2);
		params2 = iParams.params2;
	}

	void setLaserParams(const LaserParticleParameters& iParams) override {
		TwoOrdersSignalGenerator::setLaserParams(iParams);
		_signalGen2.setLaserParams(iParams);
	}

	SignalHolder generateSignal(const valVec& iTime, const Polarization& iPol) override {
		const auto oSignal0 = OneOrderSignalGenerator::generateSignal(iTime, iPol, true);
		const auto oSignal1 = _signalGen1.generateSignal(iTime, iPol, true);
		const auto oSignal2 = _signalGen2.generateSignal(iTime, iPol, true);

		SignalHolder oHolder{ {
			{_params.mode,  {oSignal0, _to, _sigma, _amplitude}},
			{params1.mode,  {oSignal1, _signalGen1._to, _signalGen1._sigma, _signalGen1._amplitude}},
			{params2.mode,  {oSignal2, _signalGen2._to, _signalGen2._sigma, _signalGen2._amplitude}}
		}};
		return oHolder;
	}


};