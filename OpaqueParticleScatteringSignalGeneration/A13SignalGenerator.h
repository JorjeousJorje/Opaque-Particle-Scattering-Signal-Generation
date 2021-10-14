#pragma once
#include <cassert>

#include "SignalHolder.h"
#include "ScatteringOrderParameters.h"
#include "LaserParticleParameters.h"

struct P0P21P22Parameters {
	ScatteringOrderParameters paramsP0;
	ScatteringOrderParameters paramsP21;
	ScatteringOrderParameters paramsP22;
};

class A13SignalGenerator: public SignalGenerator {
	P0P21P22Parameters _params;
	LaserParticleParameters _LPParams;

	struct OffsetTimes {
		double t0;
		double t1;
		double t2;
	};

	struct Amplitudes {
		double a0;
		double a1;
		double a2;
	};
	OffsetTimes _timeOffsets;

public:
	explicit A13SignalGenerator(const P0P21P22Parameters& iParams,
								const LaserParticleParameters LPParams)
	:	_params{ iParams },
		_LPParams{ LPParams },
		_timeOffsets{ initTimeOffsets() }
	{
		assert(_params.paramsP0.mode == ScatteringMode::P0);
		assert(_params.paramsP21.mode == ScatteringMode::P21);
		assert(_params.paramsP22.mode == ScatteringMode::P22);
	}


	Vector generateSignal(const Vector& iTime, const Polarization& iPol) override {
		const double sigma_squared = _LPParams.sigma * _LPParams.sigma;
		const auto [a0, a1, a2] = getPolarizationAmplitudes(iPol);


		Vector oSignal(iTime.size());

		for(auto i = 0u; i < iTime.size(); ++i) {
			const auto t0 = iTime[i] - _timeOffsets.t0;
			const auto t1 = iTime[i] - _timeOffsets.t1;
			const auto t2 = iTime[i] - _timeOffsets.t2;

			const auto p0 = a0 * std::exp(-2.0 * (t0 * t0) / sigma_squared);
			const auto p21 = a1 * std::exp(-2.0 * (t1 * t1) / sigma_squared);
			const auto p22 = a2 * std::exp(-2.0 * (t2 * t2) / sigma_squared);

			oSignal[i] = p0 + p21 + p22;
		}

		return oSignal;

	}

	SignalHolder generateSignal(const valVec& iTime, const Polarization& iPol) override {
		const double sigma_squared = _LPParams.sigma * _LPParams.sigma;
		const auto [a0, a1, a2] = getPolarizationAmplitudes(iPol);
		const auto t0 = iTime - _timeOffsets.t0;
		const auto t1 = iTime - _timeOffsets.t1;
		const auto t2 = iTime - _timeOffsets.t2;

		const auto p0 = a0 * std::exp(-2.0 * (t0 * t0) / sigma_squared);
		const auto p21 = a1 * std::exp(-2.0 * (t1 * t1) / sigma_squared);
		const auto p22 = a2 * std::exp(-2.0 * (t2 * t2) / sigma_squared);


		SignalHolder signalHolder {
			 {{ScatteringMode::P0, p0},
					{ScatteringMode::P21, p21},
					{ScatteringMode::P22, p22}} };
		return signalHolder;
	}

private:

	OffsetTimes initTimeOffsets() const {
		const double t0 = _LPParams.d / (2.0 * _LPParams.v) * std::sin(_params.paramsP0.theta);
		const double t1 = _LPParams.d / (2.0 * _LPParams.v) * std::sin(_params.paramsP21.theta);
		const double t2 = _LPParams.d / (2.0 * _LPParams.v) * std::sin(_params.paramsP22.theta);
		return { t0, t1, t2 };
	}

	Amplitudes getPolarizationAmplitudes(const Polarization& iPol) const {
		if (iPol == Polarization::one) {
			return { _params.paramsP0.ampP1, _params.paramsP21.ampP1, _params.paramsP22.ampP1 };
		}
		return { _params.paramsP0.ampP2, _params.paramsP21.ampP2, _params.paramsP22.ampP2 };
	}
};