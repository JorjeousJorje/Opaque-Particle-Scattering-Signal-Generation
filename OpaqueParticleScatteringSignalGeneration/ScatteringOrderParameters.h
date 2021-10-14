#pragma once

enum class ScatteringMode: uint8_t {
	P0,
	P21 = 21,
	P22 = 22
};

struct ScatteringOrderParameters {
	ScatteringMode mode{ ScatteringMode::P0 };
	double m{};
	std::optional<double> thetaSca{};
	double theta{};
	double ampP1{};
	double ampP2{};
};
