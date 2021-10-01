#pragma once
#include <algorithm>
#include <sciplot/Vec.hpp>
#include <limits>


#undef  min
#undef  max


namespace Utility
{
	template<class T>
	std::enable_if_t<!std::numeric_limits<T>::is_integer, bool>
		AlmostEqual(T x, T y, const double ulp = 10.0) {
		return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
			|| std::fabs(x - y) < std::numeric_limits<T>::min();

	}


	template <typename T0, typename T1>
	sciplot::Vec GenerateLinspace(T0 iX0, T1 iX1, const std::size_t iIntervalCount) {
		sciplot::Vec oLinspace(iIntervalCount + 1);
		auto step = (iX1 - iX0) / static_cast<double>(iIntervalCount);
		for (std::size_t i = 0; i <= iIntervalCount; ++i) {
			oLinspace[i] = iX0 + static_cast<double>(i) * step;
		}

		return oLinspace;
	}

	template <typename T0, typename T1>
	sciplot::Vec GenerateLinspaceWithStep(T0 iX0, T1 iX1, const double iStep) {
		auto numSteps = static_cast<std::size_t>((iX1 - iX0) / iStep);
		return GenerateLinspace(iX0, iX1, numSteps);
	}

	inline sciplot::Vec GenerateZeroArray(const std::size_t iCount) {
		sciplot::Vec oValues(iCount);
		std::fill(std::begin(oValues), std::end(oValues), 0.0);
		return oValues;
	}


	inline sciplot::Vec GenerateNumberArray(const std::size_t iCount, const double iNumber) {
		sciplot::Vec oValues(iCount);
		std::fill(std::begin(oValues), std::end(oValues), iNumber);
		return oValues;
	}
}
