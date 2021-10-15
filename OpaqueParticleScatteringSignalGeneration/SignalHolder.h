#pragma once
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <vector>

#include "ScatteringOrderParameters.h"

using valVec = std::valarray<double>;


class SignalHolder {
	std::unordered_map<ScatteringMode, valVec> _signals;

public:
	explicit SignalHolder(std::unordered_map<ScatteringMode, valVec> iSignals)
	:	_signals{ std::move(iSignals)}
	{
	}

	const valVec& operator[](const ScatteringMode& iMode) const
	{
		return _signals.at(iMode);
	}

	valVec getResultSignal() const {
		valVec oResultSignal = _signals.begin()->second;

		if (_signals.size() > 1) {
			std::for_each(std::next(_signals.begin()), _signals.end(), [&](const auto& pair)
				{
					oResultSignal += pair.second;
				});
		}

		return oResultSignal;
	}

};