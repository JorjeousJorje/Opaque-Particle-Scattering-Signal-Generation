#pragma once
#include <filesystem>
#include <fstream>

#include "Config.h"
#include "ConfigParser.h"
#include "LaserParticleParameters.h"

class LaserParticleConfig final: public Config {

public:
	LaserParticleConfig()
	:	Config{ { "d", "v", "w0", "output file" } }
	{
	}
	void read(std::istream& iStream, const char sep = '=') override {
		if (emptyConfig(iStream)) {
			return;
		}

		ConfigParser parser{};
		for(auto&& item: _order) {
			if (item != "output file") {

				if(setNumericItems(iStream, parser, item, sep)) {
					continue;
				}
				return;

			}

			if(!setOutputDirItem(iStream, parser, item, sep)) {
				return;
			}
		}

		std::cout << "<config>: received" << std::endl;
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

	bool emptyConfig(std::istream& iStream) {
		if (iStream.peek() == std::istream::traits_type::eof()) {
			std::cout << "<config>: empty config file" << std::endl;
			return true;
		}
		return false;
	}

	bool setNumericItems(std::istream& iStream, ConfigParser& iParser, const std::string& iItem, const char sep) {
		try {
			double number = std::stod(iParser.parseLine(iStream, sep));
			config_.at(iItem) = number;
			return true;
		}
		catch (const std::invalid_argument&) {
			std::cout << "<config>: item '" + iItem << "' must be numerical!" << std::endl;
			return false;
		}
		catch (const std::out_of_range&) {
			std::cout << "<config>: item '" + iItem << "' is out of the range of the result type!" << std::endl;
			return false;
		}
	}


	bool setOutputDirItem(std::istream& iStream, ConfigParser& iParser, const std::string& iItem, const char sep) {
		config_.at(iItem) = iParser.parseLine(iStream, sep);
		const auto& filePath = std::get<std::string>(config_.at(iItem));

		if (std::filesystem::directory_entry{ filePath }.exists()) {
			if (!std::filesystem::directory_entry{ filePath }.is_regular_file()) {
				if (filePath.empty()) {
					std::cout << "<config>: item 'output file' is empty" << std::endl;
				}
				else {
					std::cout << "<config>: " << filePath << " is not regular file." << std::endl;
				}
				return false;
			}
		}
		else {
			std::cout << "<config>: " << filePath << " doesn't exist. This file will be created." << std::endl;
			std::ofstream{ filePath };
		}
		return true;
	}
};
