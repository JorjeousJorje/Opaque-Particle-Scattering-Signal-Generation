#pragma once
#include <optional>
#include <cmath>

#include "OpaqueParticleModel.h"


class SnellLaw {
	RefractivePair _indexces;
public:
	SnellLaw(const RefractivePair& indxs)
	:	_indexces{ indxs }
	{
	}

	std::optional<double> getTransitionAngle(const double incidentAngle, bool outside = true) const {
		auto [n1, n2] = _indexces;

		if (!outside) {
			const auto n1_saved = n1;
			n1 = n2;
			n2 = n1_saved;
		}
		const auto result = std::asin(n1 / n2 * std::sin(incidentAngle));

		if (std::isnan(result)) {
			return std::nullopt;
		}

		return result;
	}
};
