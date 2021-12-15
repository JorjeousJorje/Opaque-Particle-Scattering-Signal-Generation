#pragma once
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <vector>

#include "ScatteringOrderParameters.h"

using valVec = std::valarray<double>;


struct SignalSample {
	valVec signal;
	double t0;
	double sigma;
	double amplitude;
};


class SignalHolder {
	std::unordered_map<ScatteringMode, SignalSample> _signals;

public:
	explicit SignalHolder(std::unordered_map<ScatteringMode, SignalSample> iSignals)
	:	_signals{ std::move(iSignals)}
	{
	}

	const valVec& operator[](const ScatteringMode& iMode) const {
		return _signals.at(iMode).signal;
	}

	[[nodiscard]] double getSignalTimePeak(const ScatteringMode& iMode) const {
		return _signals.at(iMode).t0;
	}

	[[nodiscard]] std::vector<double> getSignalTimePeaks() const {
		std::vector<double> oTimePeaks(_signals.size());
		auto time_selector = [](const auto& pair) { return pair.second.t0; };
		std::transform(_signals.begin(), _signals.end(), oTimePeaks.begin(), time_selector);
		return oTimePeaks;
	}

	[[nodiscard]] double getSignalAmplitude(const ScatteringMode& iMode) const {
		return _signals.at(iMode).amplitude;
	}

	[[nodiscard]] std::vector<double> getSignalAmplitudes() const {
		std::vector<double> oTimePeaks(_signals.size());
		auto time_selector = [](const auto& pair) { return pair.second.amplitude; };
		std::transform(_signals.begin(), _signals.end(), oTimePeaks.begin(), time_selector);
		return oTimePeaks;
	}

	[[nodiscard]] double getSigmaWidth(const ScatteringMode& iMode) const {
		return _signals.at(iMode).sigma;
	}

	[[nodiscard]] std::vector<ScatteringMode> getModes() const {
		std::vector<ScatteringMode> oModes(_signals.size());
		auto key_selector = [](const auto& pair) { return pair.first; };
		std::transform(_signals.begin(), _signals.end(), oModes.begin(), key_selector);
		return oModes;

	}

	[[nodiscard]] std::size_t signalsNum() const {
		return _signals.size();
	}

	[[nodiscard]] valVec getResultSignal() const {
		valVec oResultSignal = _signals.begin()->second.signal;
		if (_signals.size() > 1) {
			const auto adder = [&](const auto& pair) { oResultSignal += pair.second.signal; };
			std::for_each(std::next(_signals.begin()), _signals.end(), adder);
		}

		return oResultSignal;
	}

};