#pragma once
#include <random>
#include <vector>

#include "Particle.h"


class InnerParticlesGenerator {

protected:
	std::random_device _rd{};
	std::mt19937 _gen{ _rd() };
	std::uniform_real_distribution<double> _coordinateDistribution;

	std::size_t _particleNum;

public:
	virtual ~InnerParticlesGenerator() = default;

	explicit InnerParticlesGenerator(const double iA, const double iB, std::size_t iParticleNum)
	:	_coordinateDistribution{ iA, iB },
		_particleNum{ iParticleNum }
	{
	}

	virtual std::vector<ParticlePtr> generateParticles() = 0;

protected:

	virtual ParticlePtr generateParticle() = 0;
};