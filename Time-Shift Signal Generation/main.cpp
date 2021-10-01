#include <sciplot/sciplot.hpp>
#include "TimeShiftSignalGenerator.h"
#include "Utility.h"
using namespace sciplot;
using namespace Utility;


void printParticles(const OpaqueParticleModel& model) {
	const auto& particles = model.getInnerParticles();


	for (auto&& par : particles) {
		std::cout << "===========" << std::endl;
		std::cout << *par;
	}
}


using namespace std::numbers;

int main() {



	//TODO: понять, как работает 10e-6

	constexpr double laserWavelength = 450e-9;
	constexpr double mainParticleDiameter = 100e-6;
	constexpr double initialIntensity = 1.0;
	constexpr double particleMeanVelocity = 10.0;


	constexpr RefractivePair n1n2 = { 1.0, 1.34 };
	const OpaqueParticleModel model{n1n2, {0.0, 0.0}, mainParticleDiameter, particleMeanVelocity, 0, laserWavelength };

	constexpr SimulatorConfig config = { initialIntensity };

	RayTracingSimulator simulator{ model , config };
	const TimeShiftSignalGenerator signalGenerator{ simulator };


	const auto time = GenerateLinspace(0.0, 15e-6, 1000);
	const auto signal = signalGenerator.generateSignal(time);


	Plot plot;


	plot.drawCurve(time, signal);
	plot.size(800, 600);
	plot.show();




	return 0;
}