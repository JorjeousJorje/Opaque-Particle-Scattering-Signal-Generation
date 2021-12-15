#pragma once
#include "ThreeOrdersSignalGenerator.h"


struct FourOrdersParameters: ThreeOrderParameters {
	ScatteringOrderParameters params3;
};

class FourOrdersSignalGenerator: public ThreeOrdersSignalGenerator {
protected:
	ScatteringOrderParameters params3;
	OneOrderSignalGenerator _signalGen3;

public:
	explicit FourOrdersSignalGenerator(const FourOrdersParameters& iParams, const LaserParticleParameters& iLPParams)
	:	ThreeOrdersSignalGenerator{ ThreeOrderParameters(iParams), iLPParams },
		params3{ iParams.params3 },
		_signalGen3{ params3, _laserParticleParams }
	{
	}

	void setLaserParams(const LaserParticleParameters& iParams) override {
		ThreeOrdersSignalGenerator::setLaserParams(iParams);
		_signalGen3.setLaserParams(iParams);
	}

	SignalHolder generateSignal(const valVec& iTime, const Polarization& iPol) override {
		const auto oSignal0 = OneOrderSignalGenerator::generateSignal(iTime, iPol, true);
		const auto oSignal1 = _signalGen1.generateSignal(iTime, iPol, true);
		const auto oSignal2 = _signalGen2.generateSignal(iTime, iPol, true);
		const auto oSignal3 = _signalGen3.generateSignal(iTime, iPol, true);

		SignalHolder oHolder{ {
			{_params.mode,  {oSignal0, _to, _sigma, _amplitude}},
			{params1.mode, {oSignal1, _signalGen1._to, _signalGen1._sigma, _signalGen1._amplitude}},
			{params2.mode, {oSignal2, _signalGen2._to, _signalGen2._sigma, _signalGen2._amplitude}},
			{params3.mode, {oSignal3, _signalGen3._to, _signalGen3._sigma, _signalGen3._amplitude}}
		}};
		return oHolder;
	}


};