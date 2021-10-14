#pragma once
#include <map>
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


	// TODO: return ref with exception!
	valVec operator[](const ScatteringMode& iMode) const
	{
		return _signals.at(iMode);
	}
};