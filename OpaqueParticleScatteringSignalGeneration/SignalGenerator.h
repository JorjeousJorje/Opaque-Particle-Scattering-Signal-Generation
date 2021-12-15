#pragma once
#include "SignalHolder.h"
#include "LaserParticleParameters.h"


class SignalGenerator {
protected:
	LaserParticleParameters _laserParticleParams;
public:

	SignalGenerator(const LaserParticleParameters& iLPParams)
		: _laserParticleParams{ iLPParams }
	{
	}

	virtual void setLaserParams(const LaserParticleParameters& iParams) {
		_laserParticleParams = iParams;
	}

	virtual ~SignalGenerator() = default;
	virtual SignalHolder generateSignal(const valVec&, const Polarization&) = 0;
};