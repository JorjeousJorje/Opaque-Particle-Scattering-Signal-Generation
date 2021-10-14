#pragma once
#include <random>


class NumberGenerator {
protected:
	std::random_device _rd{};
	std::mt19937 _gen{ _rd() };


public:
	virtual ~NumberGenerator() = default;



	virtual double generate() = 0;
};
