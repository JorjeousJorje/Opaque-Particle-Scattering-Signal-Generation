#pragma once
#include "NumberGenerator.h"


class KsiAngleGenerator : public NumberGenerator {
	std::normal_distribution<double> _angleDistribution;

public:
	KsiAngleGenerator(const double iMean, const double iStd)
	:	_angleDistribution{ iMean , iStd }
	{
	}


	double generate() override {
		return _angleDistribution(_gen);
	}

};
