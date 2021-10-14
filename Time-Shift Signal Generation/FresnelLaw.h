#pragma once
#include <cassert>
#include <cmath>
#include <complex>

#include "Utility.h"


struct FresnelCoeffs {
	double T;
	double R;
};

class FresnelLaw {
	RefractivePair _indexces;
public:
	FresnelLaw(const RefractivePair& indxs)
		: _indexces{ indxs }
	{
	}

	FresnelCoeffs getFresnelCoefficients(const double theta_t, const double theta_i, bool outside = true) const {

		auto [n1, n2] = _indexces;

		if (!outside) {
			const auto n1_saved = n1;
			n1 = n2;
			n2 = n1_saved;
		}

		// const auto t_s = 2.0 * n1 * std::cos(theta_i) / (n1 * std::cos(theta_i) + n2 * std::cos(theta_t));
		
		const auto r_s = (n1 * std::cos(theta_i) - n2 * std::cos(theta_t)) / (n1 * std::cos(theta_i) + n2 * std::cos(theta_t));


		// const auto T = n2 * std::cos(theta_t) * t_s * t_s / (n1 * std::cos(theta_i));
		const auto R = r_s * r_s;
		const auto T = 1.0 - R;
		// assert(Utility::AlmostEqual(R + T, 1.0, 10e6));

		return { T,  R };
	}
};