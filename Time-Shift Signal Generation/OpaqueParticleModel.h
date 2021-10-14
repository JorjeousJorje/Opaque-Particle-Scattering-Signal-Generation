#pragma once
#include <vector>
#include "SphericalParticle.h"
#include "InnerSphericalParticlesGenerator.h"


struct RefractivePair {
	double _n1;
	double _n2;
};


struct OpaqueParticleModelConfig {
	Position pos;
	double diameter;
	double velocity;
	RefractivePair indexes;
	std::size_t particleNum;
	double innerParticleDiameter;
};


class OpaqueParticleModel : public SphericalParticle {
	double _velocity;
	InnerSphericalParticlesGenerator _generator;
	RefractivePair _indexes;
	std::vector<ParticlePtr> _innerParticles;

public:
	explicit OpaqueParticleModel(const OpaqueParticleModelConfig& config)
	:	SphericalParticle{ config.pos, config.diameter },
		_velocity{ config.velocity },
		_generator{ 0.0, _radius, config.particleNum, config.innerParticleDiameter },
		_indexes{ config.indexes },
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