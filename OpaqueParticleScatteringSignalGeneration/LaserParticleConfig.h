#pragma once
#include <filesystem>
#include <fstream>

#include "Config.h"
#include "ConfigParser.h"
#include "LaserParticleParameters.h"

class LaserParticleConfig final: public Config {

public:
	LaserParticleConfig(std::vector<ConfigItem> iOrder)
	:	Config{ std::move(iOrder) }
	{
	}

	double getParticleDiameter() {
		return std::get<double>(look_up("d").value());
	}

	double getParticleVelocity() {
		return std::get<double>(look_up("v").value());
	}

	double getLaserWidth() {
		return std::get<double>(look_up("w0").value());
	}

	std::string getOutputFile() {
		return std::get<std::string>(look_up("output file").value());
	}


	LaserParticleParameters getLaserParticleParameters() {
		const auto d = getParticleDiameter();
		const auto v = getParticleVelocity();
		const auto w0 = getLaserWidth();
		const auto sigma = w0 / v;

		return { d, v, w0, sigma };
	}


private:
};
