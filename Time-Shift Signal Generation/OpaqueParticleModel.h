#pragma once
#include <vector>
#include "SphericalParticle.h"
#include "InnerSphericalParticlesGenerator.h"


struct RefractivePair {
	double _n1;
	double _n2;
};


class OpaqueParticleModel : public SphericalParticle {
	double _velocity;
	InnerSphericalParticlesGenerator _generator;
	RefractivePair _indexes;
	std::vector<ParticlePtr> _innerParticles;

public:
	explicit OpaqueParticleModel(RefractivePair iIndexes , const Position& pos, const double iDiameter, const double iVelocity, std::size_t iParticleNum = 0, double iInnerParticleDiameter = 1.0)
	:	SphericalParticle{ pos, iDiameter },
		_velocity{ iVelocity },
		_generator{ 0.0, _radius, iParticleNum, iInnerParticleDiameter },
		_indexes{ iIndexes },
		_innerParticles{ _generator.generateParticles()}
	{
	}

	const RefractivePair& getRefractiveIndexes() const {
		return _indexes;
	}

	double getVelocity() const {
		return _velocity;
	}

	const auto& getInnerParticles() const {
		return _innerParticles;
	}


};