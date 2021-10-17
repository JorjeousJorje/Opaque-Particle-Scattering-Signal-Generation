#pragma once
#include <unordered_map>
#include "ScatteringOrderParameters.h"


class ParameterHolder {
	using Params = std::unordered_multimap<ScatteringMode, ScatteringOrderParameters>;
	Params _params;



public:
	explicit ParameterHolder(Params iParams)
	:	_params{ std::move( iParams) }
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
