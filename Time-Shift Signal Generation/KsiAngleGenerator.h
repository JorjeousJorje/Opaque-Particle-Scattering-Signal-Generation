#pragma once
#include <random>


class KsiAngleGenerator {
	std::random_device _rd{};
	std::mt19937 _gen{ _rd() };
	std::normal_distribution<double> _angleDistribution;




public:
	KsiAngleGenerator(const double iMean, const double iStd)
	:	_angleDistribution{ iMean , iStd }
	{
	}


	double generateAngle() {
		return _angleDistribution(_gen);
	}

};
