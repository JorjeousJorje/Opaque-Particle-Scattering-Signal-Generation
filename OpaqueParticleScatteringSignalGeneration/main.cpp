#include <sciplot/sciplot.hpp>
#include <iostream>
#include <numbers>
#include <fstream>

#include "ParametersParsers.h"
#include "Settings.h"
#include "A13SignalGenerator.h"

using namespace sciplot;
using namespace std::numbers;
using namespace Utility;


int main() {

	P0ParametersParser P0parser{};
	P21P22ParametersParser P21P22Parser{};

	std::ifstream configFile{ filePathLaserConfig };
	config.read(configFile);

	const auto laserParameters = config.getLaserParticleParameters();
	const auto paramsP0 = P0parser.parseSignalParameters(filePathParametersP0, thetaScattering).value();
	const auto paramsP21P22 = P21P22Parser.parseSignalParameters(filePathParametersP21P22, thetaScattering).value();

	const auto& paramsP21 = paramsP21P22[ScatteringMode::P21];
	const auto& paramsP22 = paramsP21P22[ScatteringMode::P22];

	A13SignalGenerator signalGenerator{ {paramsP0, paramsP21, paramsP22}, laserParameters };

	const auto time = GenerateLinspace(tStart, tEnd, 500);
	const auto signals = signalGenerator.generateSignal(time, pol);

	const auto& p0 = signals[ScatteringMode::P0];
	const auto& p21 = signals[ScatteringMode::P21];
	const auto& p22 = signals[ScatteringMode::P22];
	const auto signal = signals.getResultSignal();

	Plot plot;
	plot.size(800, 600);
	plot.xlabel("time in micro seconds");
	plot.ylabel("s(t)");
	plot.drawCurve(time, p0).label("p0").lineWidth(2);
	plot.drawCurve(time, p21).label("p21").lineWidth(2);
	plot.drawCurve(time, p22).label("p22").lineWidth(2);
	plot.save("Orders with theta scattering=" + std::to_string(thetaScattering) + ", pol=" + std::to_string(to_underlying(pol)) + ".pdf");
	plot.clear();

	plot.drawCurve(time, signal).label("p0 + p21 + p22").lineWidth(4);
	plot.save("Whole signal with theta scattering=" + std::to_string(thetaScattering) + ", pol=" + std::to_string(to_underlying(pol)) + ".pdf");
	return 0;
}