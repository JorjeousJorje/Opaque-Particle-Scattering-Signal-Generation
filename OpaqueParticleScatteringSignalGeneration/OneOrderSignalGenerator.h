#pragma once
#include "SignalGenerator.h"
#include "ScatteringOrderParameters.h"
#include "LaserParticleParameters.h"

class TwoOrdersSignalGenerator;
class ThreeOrdersSignalGenerator;
class FourOrdersSignalGenerator;
class FiveOrdersSignalGenerator;

class OneOrderSignalGenerator: public SignalGenerator {
protected:
	ScatteringOrderParameters _params;
	double _to{};
	double _sigma{};

public:
	explicit OneOrderSignalGenerator(const ScatteringOrderParameters& iParams, const LaserParticleParameters& iLPParams)
	:	SignalGenerator{ iLPParams },
		_params{iParams}
	{
	}


	SignalHolder generateSignal(const valVec& iTime, const Polarization& iPol) override {
		const auto oSignal = generateSignal(iTime, iPol, true);
		SignalHolder oHolder{{
				{_params.mode, {oSignal, _to, _sigma}}
		}};
		return oHolder;

	}

protected:

	friend TwoOrdersSignalGenerator;
	friend ThreeOrdersSignalGenerator;
	friend FourOrdersSignalGenerator;
	friend FiveOrdersSignalGenerator;

	valVec generateSignal(const valVec& iTime, const Polarization& iPol, const bool) {
		_to = _laserParticleParams.d / (2.0 * _laserParticleParams.v) * std::sin(_params.theta);
		_sigma = _laserParticleParams.sigma;
		const auto sigma_squared = _sigma * _sigma;

		const auto t_t0 = iTime - _to;
		const auto A = getPolarizationAmplitude(iPol);

		return A * std::exp(-2 * (t_t0 * t_t0) / sigma_squared);
	}

	double getPolarizationAmplitude(const Polarization& iPol) const {
		if(iPol == Polarization::one) {
			return _params.ampP1;
		}
		return _params.ampP2;
	}

};