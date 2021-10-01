#pragma once
#include <numbers>
#include <optional>
#include <cassert>


#include "OpaqueParticleModel.h"
#include "IncidentRay.h"
#include "KsiAngleGenerator.h"
#include "PathGenerator.h"
#include "Utility.h"

using namespace std::numbers;

double normalizeAngle2PI(double angle)
{
	double a = fmod(angle, 2 * pi);
	return a >= 0 ? a : a + 2 * pi;
}

double normalizeAngle(double angle)
{
	double a = fmod(angle + pi, 2 * pi);
	return a >= 0 ? (a - pi) : (a + pi);
}

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

	using IncidentRays = std::vector<IncidentRay>;

public:
	RayTracingSimulator(const OpaqueParticleModel& iModel, const SimulatorConfig& iConfig)
	:	_model{ iModel },
		_config{ iConfig },
		_angleGenerator{ _config.meanKsi, _config.stdKsi },
		_pathGenerator{ std::isinf(_config.E_L) ? 0.5 : 1.0 / _config.E_L }
	{
	}

	std::optional<double> simulateRayTracing(const double iTimePoint) {

		const auto v = _model.getVelocity();
		const auto d = _model.getDiameter();


		const auto theta_i = std::asin(2.0 * v * iTimePoint / d);

		if (std::isnan(theta_i)) {
			return std::nullopt;
		}

		auto [rays, I_0_out] = initializeRays(theta_i);
		double resultIntensity = I_0_out * static_cast<double>(rays.size());

		for (auto& ray : rays) {

			auto& [r_k, phi_k, I_k, theta_k] = ray;

			while (!Utility::AlmostEqual(I_k, _config._intensityThreshold) && I_k > _config._intensityThreshold) {
				/*phi_k = normalizeAngle(phi_k);
				theta_k = normalizeAngle2PI(theta_k);*/

				// phi_k = normalizeAngle2PI(phi_k);
				// theta_k = normalizeAngle2PI(theta_k);

				/*phi_k = normalizeAngle2PI(phi_k);
				theta_k = normalizeAngle(theta_k);*/

				if (!std::isinf(_config.E_L)) {
					simulateInternalScatterings(ray);
				}

				simulateInnerSurfaceReflection(ray, resultIntensity);

			}
		}

		return resultIntensity;

	}


private:
	void simulateInternalScatterings(IncidentRay& iRay) {
		const auto d = _model.getDiameter();

		auto& [r_k, phi_k, I_k, theta_k] = iRay;
		while (true) {

			const auto beta_k = phi_k - std::numbers::pi_v<double> - theta_k;

			const auto L_k = _pathGenerator.generatePath();
			const auto r_k1 = std::sqrt(L_k * L_k + r_k * r_k - 2 * r_k * L_k * std::cos(beta_k));
			const auto alpha = std::acos((r_k - L_k * std::cos(beta_k)) / r_k1);

			if (Utility::AlmostEqual(r_k1, d / 2.0) || r_k1 > d / 2.0) {
				break;
			}

			if (!_model.getInnerParticles().empty()) {
				const auto ksi = _angleGenerator.generateAngle();
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
		const auto theta_prev = theta_k;
		theta_k = pi - 2.0 * (theta_k + phi_k);


		phi_k = normalizeAngle(phi_k);
		theta_k = normalizeAngle2PI(theta_k);

		/*phi_k = normalizeangle2pi(phi_k);
		theta_k = normalizeangle(theta_k);*/

		/*phi_k = normalizeAngle(phi_k);
		theta_k = normalizeAngle(theta_k);*/

		const auto [T, R] = getFresnelCoefficients(theta_k, theta_prev, false);

		iResultIntensity += I_k * T;
		I_k *= R;
		r_k = r_k1;
	}

	std::pair<IncidentRays, double> initializeRays(const double iTheta_i) const {
		IncidentRays oRays(_config._incidentRaysNumber);
		auto [n1, n2] = _model.getRefractiveIndexes();


		const auto beta_0 = std::asin(n1 / n2 * std::sin(iTheta_i));
		const auto theta_0 = iTheta_i - beta_0;
		const auto phi_0 = pi + iTheta_i;
		const auto r_0 = _model.getRadius();

		const auto [T, R] = getFresnelCoefficients(theta_0, iTheta_i);

		const auto I_0 = _config._initialIntensity * T;

		auto generateFunc = [=]() -> IncidentRay { return { r_0, phi_0, I_0 , theta_0 }; };
		std::generate(oRays.begin(), oRays.end(), generateFunc);

		return { oRays, _config._initialIntensity * R };

	}

	std::pair<double, double> getFresnelCoefficients(const double theta_t, const double theta_i, bool outside = true) const {


		auto [n1, n2] = _model.getRefractiveIndexes();

		if(!outside) {
			const auto n1_saved = n1;
			n1 = n2;
			n2 = n1_saved;
		}

		const auto t_s = 2.0 * n1 * std::cos(theta_i) / (n1 * std::cos(theta_i) + n2 * std::cos(theta_t));
		const auto r_s = (n1 * std::cos(theta_i) - n2 * std::cos(theta_t)) / (n1 * std::cos(theta_i) + n2 * std::cos(theta_t));


		assert(Utility::AlmostEqual(t_s - r_s, 1.0, 10e6));

		const auto T = n2 * std::cos(theta_t) * t_s * t_s / (n1 * std::cos(theta_i));
		const auto R = r_s * r_s;

		return { T,  R};
	}
};