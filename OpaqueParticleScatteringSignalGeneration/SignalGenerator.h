#pragma once
#include <valarray>

#include "SignalHolder.h"
#include "LaserParticleParameters.h"


enum class Polarization: uint8_t {
	one = 1,
	two = 2
};

class SignalGenerator {
protected:
	LaserParticleParameters _laserParticleParams;
public:

	SignalGenerator(const LaserParticleParameters& iLPParams)
		: _laserParticleParams{ iLPParams }
	{
	}

	virtual ~SignalGenerator() = default;
	virtual SignalHolder generateSignal(const valVec&, const Polarization&) = 0;
};