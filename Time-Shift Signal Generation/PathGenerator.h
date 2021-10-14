#pragma once
#include "NumberGenerator.h"


class PathGenerator: public NumberGenerator {
	std::exponential_distribution<double> _pathDistribution;

public:
	PathGenerator(const double iAlpha)
		: _pathDistribution{ iAlpha }
	{
	}


	double generate() override {
		return _pathDistribution(_gen);
	}

};
