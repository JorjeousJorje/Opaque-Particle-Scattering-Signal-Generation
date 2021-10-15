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

public:
	OneOrderSignalGenerator(const ScatteringOrderParameters& iParams, const LaserParticleParameters& iLPParams)
	:	SignalGenerator{ iLPParams },
		_params{iParams}
	{
	}


	SignalHolder generateSignal(const valVec& iTime, const Polarization& iPol) override {
		const auto oSignal = generateSignal(iTime, iPol, true);
		SignalHolder holder{{
				{_params.mode, oSignal}
		}};
		return holder;

	}

protected:

	friend TwoOrdersSignalGenerator;
	friend ThreeOrdersSignalGenerator;
	friend FourOrdersSignalGenerator;
	friend FiveOrdersSignalGenerator;

	valVec generateSignal(const valVec& iTime, const Polarization& iPol, const bool) const {
		const auto t0 = _laserParticleParams.d / (2.0 * _laserParticleParams.v) * std::sin(_params.theta);
		const auto sigma_squared = _laserParticleParams.sigma * _laserParticleParams.sigma;

		const auto t_t0 = iTime - t0;
		const auto A = getPolarizationAmplitude(iPol);

		const auto oSignal = A * std::exp(-2 * (t_t0 * t_t0) / sigma_squared);

		return oSignal;
	}

	double getPolarizationAmplitude(const Polarization& iPol) const {
		if(iPol == Polarization::one) {
			return _params.ampP1;
		}
		return _params.ampP2;
	}

};