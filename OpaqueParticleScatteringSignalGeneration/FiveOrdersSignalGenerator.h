#pragma once
#include "FourOrdersSignalGenerator.h"

struct FiveOrdersParameters: FourOrdersParameters {
	ScatteringOrderParameters params4;
};

class FiveOrdersSignalGenerator: public FourOrdersSignalGenerator {
protected:
	ScatteringOrderParameters params4;
	OneOrderSignalGenerator _signalGen4;

public:
	explicit FiveOrdersSignalGenerator(const FiveOrdersParameters& iParams, const LaserParticleParameters& iLPParams)
	:	FourOrdersSignalGenerator{ FourOrdersParameters(iParams), iLPParams },
		params4{ iParams.params4 },
		_signalGen4{ params4, _laserParticleParams }
	{
	}

	void setLaserParams(const LaserParticleParameters& iParams) override {
		FourOrdersSignalGenerator::setLaserParams(iParams);
		_signalGen4.setLaserParams(iParams);
	}

	SignalHolder generateSignal(const valVec& iTime, const Polarization& iPol) override {
		const auto oSignal0 = OneOrderSignalGenerator::generateSignal(iTime, iPol, true);
		const auto oSignal1 = _signalGen1.generateSignal(iTime, iPol, true);
		const auto oSignal2 = _signalGen2.generateSignal(iTime, iPol, true);
		const auto oSignal3 = _signalGen3.generateSignal(iTime, iPol, true);
		const auto oSignal4 = _signalGen3.generateSignal(iTime, iPol, true);

		SignalHolder oHolder{ {
			{_params.mode, {oSignal0, _to, _sigma, _amplitude}},
			{params1.mode, {oSignal1, _signalGen1._to, _signalGen1._sigma, _signalGen1._amplitude}},
			{params2.mode, {oSignal2, _signalGen2._to, _signalGen2._sigma, _signalGen2._amplitude}},
			{params3.mode, {oSignal3, _signalGen3._to, _signalGen3._sigma, _signalGen3._amplitude}},
			{params4.mode, {oSignal4, _signalGen4._to, _signalGen4._sigma, _signalGen4._amplitude}}
		}};
		return oHolder;
	}


};