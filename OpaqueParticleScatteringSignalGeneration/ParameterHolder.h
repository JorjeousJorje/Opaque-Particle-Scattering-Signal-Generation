#pragma once
#include <unordered_map>
#include "ScatteringOrderParameters.h"


class ParameterHolder {
	using Params = std::unordered_map<ScatteringMode, ScatteringOrderParameters>;
	Params _params;



public:
	explicit ParameterHolder(Params iParams)
	:	_params{ std::move( iParams) }
	{
	}

	const ScatteringOrderParameters& operator[](const ScatteringMode& iMode) const
	{
		return _params.at(iMode);
	}

	bool empty() const {
		return _params.empty();
	}
};
