#pragma once
#include <optional>
#include <vector>

#include "ScatteringOrderParameters.h"


class ParameterHolder {
	using Params = std::map<ScatteringMode, ScatteringOrderParameters>;
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
};
