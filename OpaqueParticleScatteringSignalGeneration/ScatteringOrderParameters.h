#pragma once
#include <optional>

enum class Polarization : uint8_t {
	one = 1,
	two = 2
};

enum class ScatteringMode: uint8_t {
	P0,
	P1,
	P21 = 21,
	P22 = 22,
	P23 = 23,
	P31 = 31,
	P32 = 32
};

struct ScatteringOrderParameters {


	ScatteringMode mode{ ScatteringMode::P0 };
	double m{};
	std::optional<double> thetaSca{};
	double theta{};
	double ampP1{};
	double ampP2{};
};


class ParameterHolder {
	using Params = std::unordered_multimap<ScatteringMode, ScatteringOrderParameters>;
	Params _params;



public:
	explicit ParameterHolder(Params iParams)
		: _params{ std::move(iParams) }
	{
	}

	const ScatteringOrderParameters& operator[](const ScatteringMode& iMode) const
	{
		const auto& oParams = _params.find(iMode)->second;
		return oParams;
	}

	std::vector<ScatteringOrderParameters> operator()(const ScatteringMode& iMode) const
	{
		const auto itRange = _params.equal_range(iMode);
		std::vector<ScatteringOrderParameters> oParams(std::distance(itRange.first, itRange.second));

		auto param_selector = [](const auto& pair) { return pair.second; };
		std::transform(itRange.first, itRange.second, oParams.begin(), param_selector);
		return oParams;
	}

	bool empty() const {
		return _params.empty();
	}
};