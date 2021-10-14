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


	std::valarray<double> generateSignal(const std::valarray<double>& iThetas) const {
		std::valarray<double> oSignal(iThetas.size());


		for(auto i = 0u; i < iThetas.size(); ++i) {

			auto result = _simulator.simulateRayTracing(iThetas[i]);
			if(result.has_value()) {
				oSignal[i] = result.value();
			}
		}

		return oSignal;
	}

	std::valarray<double> generateSignalTime(const std::valarray<double>& iTime) const {
		std::valarray<double> oSignal(iTime.size());
		std::valarray<double> thetas(iTime.size());
		std::valarray<double> ress(iTime.size());

		auto step = iTime[1] - iTime[0];
		double offset = 0.0;
		double theta_i = 0.0;
		//auto nan_theta = 0.0;
		//auto current_delta = pi / 2.0;
		for (auto i = 0u; i < iTime.size(); ++i) {
			// + offset * pi / 2.0;
			// - offset * pi / 2.0;

			theta_i = std::asin(2.0 * 10.0 * iTime[i] / 100e-6 - offset) + pi / 2.0;
			if (std::isnan(theta_i)) {
				offset += 2.0 * 10.0 * iTime[i] / 100e-6;
				theta_i = std::asin(2.0 * 10.0 * iTime[i - 1] / 100e-6 - offset) + pi / 2.0;
			}

			auto result = _simulator.simulateRayTracing(theta_i);
			if (result.has_value()) {
				oSignal[i] = result.value();
			}
		}

		return oSignal;
	}

};