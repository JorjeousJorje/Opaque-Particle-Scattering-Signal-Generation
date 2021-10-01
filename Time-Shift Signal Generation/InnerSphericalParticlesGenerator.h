#pragma once
#include <numbers>
#include <algorithm>

#include "SphericalParticle.h"
#include "InnerParticlesGenerator.h"





class InnerSphericalParticlesGenerator : public InnerParticlesGenerator {

	double _innerParticleDiameter;
	std::uniform_real_distribution<double> _angleDistribution{ 0.0, 360.0 };
	
public:
	explicit InnerSphericalParticlesGenerator(const double iA, const double iB, std::size_t iParticleNum = 0, const double iInnerParticleDiameter = 1.0)
	:	InnerParticlesGenerator{ iA, iB, iParticleNum },
		_innerParticleDiameter{ iInnerParticleDiameter }
	{
	}


	std::vector<ParticlePtr> generateParticles() override {
		std::vector<ParticlePtr> oParticles(_particleNum);
		auto func = [&] { return generateParticle(); };
		std::generate(oParticles.begin(), oParticles.end(), func);
		return oParticles;
	}

private:

	ParticlePtr generateParticle() override {

		const auto r = _coordinateDistribution(_gen);
		const auto phiAngle = _angleDistribution(_gen);

		Position oPos{ r, phiAngle };
		return std::make_unique<SphericalParticle>(oPos, _innerParticleDiameter);
	}
};