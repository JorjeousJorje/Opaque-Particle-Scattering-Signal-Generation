﻿#include <sciplot/sciplot.hpp>
#include <numbers>

#include "ParametersParsers.h"
#include "Settings.h"
#include "A13SignalGenerator.h"

using namespace sciplot;
using namespace std::numbers;
using namespace Utility;


int main() {

	P0ParametersParser P0parser{};
	P21P22ParametersParser P21P22Parser{};

	LaserParticleParameters laserParameters{
		100.0,
		10.0,
		10.0,
	};

	const auto paramsP0 = P0parser.parseSignalParameters(filePathParametersP0, thetaScattering).value();
	const auto paramsP21P22 = P21P22Parser.parseSignalParameters(filePathParametersP21P22, thetaScattering);

	const auto& paramsP21 = paramsP21P22[ScatteringMode::P21];
	const auto& paramsP22 = paramsP21P22[ScatteringMode::P22];

	A13SignalGenerator signalGenerator13{ {{paramsP0, paramsP21}, paramsP22}, laserParameters };

	const auto time = GenerateLinspace(tStart, tEnd, 1000);
	const auto signals = signalGenerator13.generateSignal(time, pol);

	const auto& p0 = signals[ScatteringMode::P0];
	const auto& p21 = signals[ScatteringMode::P21];
	const auto& p22 = signals[ScatteringMode::P22];
	const auto signal = signals.getResultSignal();

	Plot plot;
	plot.size(800, 600);
	plot.xlabel("time in micro seconds").fontSize(25);
	plot.ylabel("s(t)").fontSize(25);
	plot.drawCurve(time, p0).label("p0").lineWidth(3);
	plot.drawCurve(time, p21).label("p21").lineWidth(3);
	plot.drawCurve(time, p22).label("p22").lineWidth(3);
	plot.save("Orders with theta scattering=" + std::to_string(thetaScattering) + ", pol=" + std::to_string(to_underlying(pol)) + ".png");

	plot.drawCurve(time, signal).label("p0 + p21 + p22").lineWidth(4);
	plot.show();
	plot.save("Whole signal with theta scattering=" + std::to_string(thetaScattering) + ", pol=" + std::to_string(to_underlying(pol)) + ".pdf");
	return 0;
}