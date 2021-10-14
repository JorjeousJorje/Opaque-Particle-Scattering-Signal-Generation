#include <sciplot/sciplot.hpp>
#include <ranges>

#include "TimeShiftSignalGenerator.h"
#include "Utility.h"
using namespace sciplot;
using namespace Utility;


using namespace std::numbers;
using namespace std::ranges;

constexpr double d = 100e-6;
constexpr double v = 10.0;
constexpr RefractivePair n1n2 = { 1.0, 1.34 };
constexpr std::size_t numParticles = 2;
constexpr double laserWavelength = 450e-9;


constexpr double initialIntensity = 1.0;

constexpr OpaqueParticleModelConfig modelConfig = {
		{0.0, 0.0},
		d,
		v,
		n1n2,
		numParticles,
		laserWavelength
};

constexpr SimulatorConfig simulatorConfig = {
		initialIntensity,
		0.0,
		0.5,
		std::numeric_limits<double>::infinity(),
		1
};

int main() {
	const OpaqueParticleModel model{ modelConfig };
	RayTracingSimulator simulator{ model , simulatorConfig };
	const TimeShiftSignalGenerator signalGenerator{ simulator };


	// const auto time = GenerateLinspace(0.0, 15e-6, 200);
	 auto thetas = GenerateLinspaceWithStep(0.0, 3.0 / 2.0 * pi, 10e-2);


	/*Vec thetas(thetas1.size() + thetas2.size());
	copy(thetas1, std::begin(thetas));
	copy(thetas2, std::begin(thetas) + thetas1.size());*/
	const auto signal = signalGenerator.generateSignal(thetas);


	Plot plot;
	plot.drawCurveWithPoints(thetas, signal);
	plot.size(800, 600);
	plot.show();


	return 0;
}