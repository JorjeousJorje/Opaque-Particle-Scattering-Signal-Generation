#pragma once
#include <memory>
#include <iostream>

class Particle;
using ParticlePtr = std::unique_ptr<Particle>;

class OpaqueParticleModel;

struct Position {
	double _r;
	double _phiAngle;
};

class Particle {
protected:
	Position _pos;

public:
	virtual ~Particle() = default;

	Particle(const Position& pos) : _pos{pos}
	{
	}

	const Position& getPosition() const {
		return _pos;
	}

	friend std::ostream& operator<< (std::ostream& stream, const Particle& particle) {
		particle.print(stream);
		return stream;
	}

protected:
	virtual void print(std::ostream& stream) const = 0;
};