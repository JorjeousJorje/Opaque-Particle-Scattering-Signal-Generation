#pragma once
#include <valarray>

class SignalHolder;
using valVec = std::valarray<double>;
using Vector = std::vector<double>;

enum class Polarization: uint8_t {
	one = 1,
	two = 2
};

class SignalGenerator {
public:
	virtual ~SignalGenerator() = default;
	virtual SignalHolder generateSignal(const valVec&, const Polarization&) = 0;
	virtual Vector generateSignal(const Vector&, const Polarization&) = 0;
};