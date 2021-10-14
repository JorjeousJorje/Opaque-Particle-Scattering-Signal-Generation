#pragma once
#include <numbers>
#include <optional>
#include <cassert>

#include "SnellLaw.h"
#include "FresnelLaw.h"
#include "OpaqueParticleModel.h"
#include "IncidentRay.h"
#include "KsiAngleGenerator.h"
#include "PathGenerator.h"
#include "Utility.h"

using namespace std::numbers;


struct SimulatorConfig {
	double _initialIntensity;
	double meanKsi = 0.0;
	double stdKsi = 1.0;
	double E_L = std::numeric_limits<double>::infinity();
	std::size_t _incidentRaysNumber = 1;
	double _intensityThreshold = 0.01 * _initialIntensity;
};


class RayTracingSimulator {

	const OpaqueParticleModel& _model;
	SimulatorConfig _config;
	KsiAngleGenerator _angleGenerator;
	PathGenerator _pathGenerator;
	SnellLaw _snellLaw{ _model.getRefractiveIndexes() };
	FresnelLaw _fresnelLaw{ _model.getRefractiveIndexes() };

	using IncidentRays = std::vector<IncidentRay>;

	std::pair<IncidentRays, double> initializeRays(const double iTheta_i) const {
		IncidentRays oRays(_config._incidentRaysNumber);


		const auto beta_0 = _snellLaw.getTransitionAngle(iTheta_i).value();
		const auto theta_0 = iTheta_i - beta_0;
		const auto phi_0 = pi + iTheta_i;
		const auto r_0 = _model.getRadius();

		const auto [T, R] = _fresnelLaw.getFresnelCoefficients(beta_0, iTheta_i);

		const auto I_0 = _config._initialIntensity * T;

		auto generateFunc = [=]() -> IncidentRay { return { r_0, phi_0, I_0 , theta_0 }; };
		std::generate(oRays.begin(), oRays.end(), generateFunc);

		return { oRays, _config._initialIntensity * R };

	}

public:
	RayTracingSimulator(const OpaqueParticleModel& iModel, const SimulatorConfig& iConfig)
	:	_model{ iModel },
		_config{ iConfig },
		_angleGenerator{ _config.meanKsi, _config.stdKsi },
		_pathGenerator{ std::isinf(_config.E_L) ? 0.5 : 1.0 / _config.E_L }
	{
	}

	std::optional<double> simulateRayTracing(const double iTheta) {

		if (std::isnan(iTheta)) {
			return std::nullopt;
		}

		auto [rays, I_0_out] = initializeRays(iTheta);
		double resultIntensity = I_0_out * static_cast<double>(rays.size());

		for (auto& ray : rays) {

			auto& [r_k, phi_k, I_k, theta_k] = ray;

			while (I_k > _config._intensityThreshold && !Utility::AlmostEqual(I_k, _config._intensityThreshold, 10e6)) {

				if (!std::isinf(_config.E_L)) {
					simulateInternalScatterings(ray);
				}

				simulateInnerSurfaceReflection(ray, resultIntensity);

				if (std::isnan(phi_k) || std::isnan(theta_k) || std::isnan(r_k)) {
					break;
				}

			}
		}

		return resultIntensity;

	}


private:

	void simulateInternalScatterings(IncidentRay& iRay) {
		const auto d = _model.getDiameter();

		auto& [r_k, phi_k, I_k, theta_k] = iRay;
		while (true) {

			if (std::isnan(phi_k) || std::isnan(theta_k) || std::isnan(r_k)) {
				break;
			}

			const auto beta_k = phi_k - pi - theta_k;

			const auto L_k = _pathGenerator.generate();
			const auto r_k1 = std::sqrt(L_k * L_k + r_k * r_k - 2 * r_k * L_k * std::cos(beta_k));
			double a = (r_k - L_k * std::cos(beta_k)) / r_k1;
			const auto alpha = std::acos((r_k - L_k * std::cos(beta_k)) / r_k1);

			if (std::isnan(alpha)) {
				std::cout << "alpha is NAN!" << std::endl;
				break;
			}

			if (r_k1 > d / 2.0  || Utility::AlmostEqual(r_k1, d / 2.0, 10e3)) {
				break;
			}

			if (!_model.getInnerParticles().empty()) {
				const auto ksi = _angleGenerator.generate();
				theta_k += ksi;
			}

			phi_k += alpha;
			r_k = r_k1;
		}
	}

	void simulateInnerSurfaceReflection(IncidentRay& iRay, double& iResultIntensity) const {
		const auto d = _model.getDiameter();

		auto& [r_k, phi_k, I_k, theta_k] = iRay;


		const auto r_k1 = d / 2.0;
		phi_k = theta_k - std::asin(std::sin(theta_k - phi_k) * r_k / r_k1);

		const auto incidentAngle = theta_k - phi_k;
		const auto transitionAngle = _snellLaw.getTransitionAngle(incidentAngle, false);

		if(transitionAngle.has_value()) {
			const auto [T, R] = _fresnelLaw.getFresnelCoefficients(transitionAngle.value(), incidentAngle, false);
			iResultIntensity += I_k * T;
			I_k *= R;
		}

		theta_k = pi - 2.0 * (theta_k - phi_k);
		r_k = r_k1;

		if (std::isnan(phi_k)) {
			std::cout << "phi_k is NAN!" << std::endl;
		}
		if (std::isnan(theta_k)) {
			std::cout << "theta_k is NAN!" << std::endl;
		}

	}
};