#pragma once
#include <map>
#include <numeric>
#include <optional>
#include <vector>

#include "ScatteringOrderParameters.h"
#include "SignalGenerator.h"

class SignalHolder {
	std::map<ScatteringMode, valVec> _signals;

public:
	explicit SignalHolder(std::map<ScatteringMode, valVec> iSignals)
	:	_signals{ std::move(iSignals)}
	{
	}

	const valVec& operator[](const ScatteringMode& iMode) const
	{
		return _signals.at(iMode);
	}

	valVec getResultSignal() const {
		valVec oResultSignal = _signals.begin()->second;
		std::for_each(std::next(_signals.begin()), _signals.end(), [&](const auto& pair)
		{
				oResultSignal += pair.second;
		});
		return oResultSignal;
	}

};