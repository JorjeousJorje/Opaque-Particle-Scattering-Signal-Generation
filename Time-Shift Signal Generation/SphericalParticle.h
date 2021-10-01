#pragma once
#include <colorconsole/color.hpp>

#include "Particle.h"


class SphericalParticle : public Particle {
protected:
	double _diameter;
	double _radius{ _diameter / 2.0 };

public:
	SphericalParticle(const Position& pos, const double iDiameter) : Particle{ pos }, _diameter{ iDiameter }
	{
	}

	double getRadius() const {
		return _radius;
	}

	double getDiameter() const {
		return _diameter;
	}

protected:

	void print(std::ostream& stream) const override {
		stream << dye::aqua("Size -> ") + "[";
		stream << dye::light_aqua("Radius: ") + std::to_string(_radius);
		stream << dye::light_green(" Diameter: ") + std::to_string(_diameter) + "]" << std::endl;

		stream << dye::purple("Position -> ") + "[";
		stream << dye::light_aqua("r: ") + std::to_string(_pos._r);
		stream << dye::light_green(" phi: ") + std::to_string(_pos._phiAngle) + "]" << std::endl;
	}
};


