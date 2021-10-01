#pragma once
#include <random>


class PathGenerator {
	std::random_device _rd{};
	std::mt19937 _gen{ _rd() };
	std::exponential_distribution<double> _pathDistribution;




public:
	PathGenerator(const double iAlpha)
		: _pathDistribution{ iAlpha }
	{
	}


	double generatePath() {
		return _pathDistribution(_gen);
	}

};
