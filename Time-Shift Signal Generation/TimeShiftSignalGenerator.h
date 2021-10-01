#pragma once
#include <valarray>
#include <vector>

#include "RayTracingSimulator.h"


class TimeShiftSignalGenerator {
	RayTracingSimulator& _simulator;



public:
	TimeShiftSignalGenerator(RayTracingSimulator& iSimulator) : _simulator{ iSimulator }
	{
	}


	std::valarray<double> generateSignal(const std::valarray<double>& iTime) const {
		std::valarray<double> oSignal(iTime.size());


		for(auto i = 0u; i < iTime.size(); ++i) {

			auto result = _simulator.simulateRayTracing(iTime[i]);
			if(result.has_value()) {
				oSignal[i] = result.value();
			}
		}

		return oSignal;
	}

	std::valarray<double> generateSignal(const std::vector<double>& iTime) const {
		std::valarray<double> oSignal(iTime.size());


		for (auto i = 0u; i < iTime.size(); ++i) {

			auto result = _simulator.simulateRayTracing(iTime[i]);
			if (result.has_value()) {
				oSignal[i] = result.value();
			}
		}

		return oSignal;
	}


};